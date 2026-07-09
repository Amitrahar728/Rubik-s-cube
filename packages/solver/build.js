const { execSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const isWindows = process.platform === 'win32';
const ext = isWindows ? '.exe' : '';
const outputName = `solver${ext}`;

fs.mkdirSync(path.join(__dirname, 'bin'), { recursive: true });

const cmd = `g++ -O3 -std=c++17 src/main.cpp src/cubiecube.cpp src/coordcube.cpp src/search.cpp -o bin/${outputName}`;
console.log(`Running: ${cmd}`);

try {
  execSync(cmd, { stdio: 'inherit', cwd: __dirname });
  console.log('C++ Solver compiled successfully.');
} catch (error) {
  console.error('Compilation failed:', error);
  process.exit(1);
}
