# **3D Rubik’s Cube Monorepo**

A modern monorepo restructuring of the 3D Rubik's Cube solver application. This project divides the codebase into a modular architecture powered by **Turborepo** as the build orchestrator and **pnpm** as the package manager.

The core solver uses a high-performance C++ implementation of **Herbert Kociemba's Two-Phase algorithm**, which runs as a persistent background process bridged by local Node.js Express server to communicate with the React-based 3D frontend.

---

## **Directory Structure**

```
/ (root)
  ├── package.json          # Root workspace configuration
  ├── pnpm-workspace.yaml   # Workspace directories definition
  ├── turbo.json            # Turborepo task pipeline configuration
  │
  ├── apps/
  │   ├── web/              # React & Vite 3D Rubik's Cube frontend (preserves original UI)
  │   └── api/              # Node.js Express bridge server running the C++ solver subprocess
  │
  └── packages/
      └── solver/           # High-performance C++ solver logic compiled natively via g++
```

---

## **Prerequisites**

To compile and run this project, make sure you have:
1. **Node.js** (v18 or higher)
2. **pnpm** (installed globally: `npm install -g pnpm`)
3. **g++** compiler (MSYS2 or MinGW on Windows, or GCC/Clang on macOS/Linux) available in your system `PATH`.

---

## **Getting Started**

Follow these simple terminal commands to download dependencies, build the C++ logic core, and spin up the development servers.

### **1. Install Dependencies**
Install all workspace dependencies at once from the root:
```bash
pnpm install
```

### **2. Build the Workspace**
Build all applications and compile the C++ solver executable:
```bash
pnpm build
```
*This command runs the compilation of the C++ solver into `packages/solver/bin/solver` (or `solver.exe` on Windows) and generates the static assets for the frontend.*

### **3. Run Development Servers**
Start both the Vite frontend and the backend API server in parallel:
```bash
pnpm dev
```
- **Vite React UI**: `http://localhost:5173`
- **Express API & C++ Solver**: `http://localhost:3001`

Simply open `http://localhost:5173` in your browser, scramble the cube, and click **SOLVE** to see the C++ solver execute and visually solve the cube in real time!
