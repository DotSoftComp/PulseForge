## 📂 Repository Structure

- **`PulseEngine/`**  
  Core of the engine.  
  - Contains **all source code**.  
  - This is the directory from which you should run tasks defined in `tasks.json` (build, run, debug, etc.).

- **`PulseForgeExecutable/`**  
  Packaging system.  
  - Uses **Inno Setup** to generate an installer.  
  - Produces an executable that installs Universe Engine on the user’s PC.

- **`Tools/`**  
  Utilities supporting development.  
  - Scripts and helper programs for the engine’s ecosystem.  
  - Example: generating DLLs from scripts, automation helpers, etc.  

---

## 🛠️ Development Workflow

1. **Work inside `PulseEngine/`**  
   - All engine code is here.  
   - Use the configured tasks (`tasks.json`) to build, run, and debug.

2. **Build the installer with `PulseForgeExecutable/`**  
   - After building the engine, create a distributable installer with Inno Setup.  
   - This step is optional and mostly for release packaging.

3. **Use `Tools/` for auxiliary tasks**  
   - Run tools as needed to automate or extend the build pipeline.  

---

## 🚀 Getting Started

### Prerequisites
- **C++17** compliant compiler  
- [Inno Setup](https://jrsoftware.org/isinfo.php) (only if you need to build the installer)  

### Build Instructions
copy the engine into **`PulseForgeExecutable/CreateExecutable`** and run the CreateExo file.