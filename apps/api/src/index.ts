import express from 'express';
import cors from 'cors';
import { spawn, ChildProcessWithoutNullStreams } from 'child_process';
import path from 'path';
import fs from 'fs';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const app = express();
app.use(cors());
app.use(express.json());

// Find the solver binary
const isWindows = process.platform === 'win32';
const ext = isWindows ? '.exe' : '';
const solverPath = path.resolve(__dirname, '../../../packages/solver/bin', `solver${ext}`);

console.log('Solver path:', solverPath);

let solverProcess: ChildProcessWithoutNullStreams | null = null;
let isReady = false;
const requestQueue: { facelets: string; resolve: (val: string) => void; reject: (err: Error) => void }[] = [];
let currentRequest: { resolve: (val: string) => void; reject: (err: Error) => void } | null = null;

function startSolver() {
  if (!fs.existsSync(solverPath)) {
    console.warn(`[API] Solver binary not found at ${solverPath}. It will be built on first monorepo build.`);
    // Don't crash immediately, wait to see if it becomes available or allow devs to compile.
    setTimeout(startSolver, 5000);
    return;
  }

  console.log('[API] Starting C++ solver process...');
  solverProcess = spawn(solverPath);

  let stdoutBuffer = '';

  solverProcess.stdout.on('data', (data) => {
    stdoutBuffer += data.toString();
    const lines = stdoutBuffer.split('\n');
    stdoutBuffer = lines.pop() || ''; // Keep the last incomplete line

    for (const line of lines) {
      const trimmedLine = line.trim();
      console.log(`[Solver stdout]: ${trimmedLine}`);
      if (trimmedLine === 'READY') {
        isReady = true;
        console.log('[API] C++ Solver is ready!');
        processQueue();
      } else if (trimmedLine.startsWith('SOLUTION: ')) {
        const sol = trimmedLine.substring(10);
        if (currentRequest) {
          currentRequest.resolve(sol);
          currentRequest = null;
        }
        processQueue();
      } else if (trimmedLine.startsWith('ERROR: ')) {
        const err = trimmedLine.substring(7);
        if (currentRequest) {
          currentRequest.reject(new Error(err));
          currentRequest = null;
        }
        processQueue();
      }
    }
  });

  solverProcess.stderr.on('data', (data) => {
    console.error(`[Solver stderr]: ${data.toString().trim()}`);
  });

  solverProcess.on('close', (code) => {
    console.log(`[API] Solver process exited with code ${code}`);
    isReady = false;
    solverProcess = null;
    currentRequest = null;
    // Attempt to restart
    setTimeout(startSolver, 3000);
  });
}

function processQueue() {
  if (!isReady || !solverProcess || currentRequest || requestQueue.length === 0) {
    return;
  }

  const req = requestQueue.shift();
  if (req) {
    currentRequest = { resolve: req.resolve, reject: req.reject };
    console.log(`[API] Solving cube: ${req.facelets}`);
    solverProcess.stdin.write(`SOLVE ${req.facelets}\n`);
  }
}

app.post('/api/solve', (req, res) => {
  const { cube } = req.body;
  if (!cube || typeof cube !== 'string') {
    return res.status(400).json({ error: 'Missing or invalid cube state' });
  }

  if (!solverProcess || !isReady) {
    return res.status(503).json({ error: 'Solver backend is initializing or not running' });
  }

  new Promise<string>((resolve, reject) => {
    requestQueue.push({ facelets: cube, resolve, reject });
    processQueue();
  })
    .then((solution) => {
      res.json({ result: solution || null });
    })
    .catch((err) => {
      res.status(500).json({ error: err.message });
    });
});

app.get('/api/init', (req, res) => {
  res.json({ ready: isReady });
});

const PORT = process.env.PORT || 3001;
app.listen(PORT, () => {
  console.log(`[API] Server listening on port ${PORT}`);
  startSolver();
});
