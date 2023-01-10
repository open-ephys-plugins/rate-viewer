# Rate Viewer

![Rate Viewer in action](https://open-ephys.github.io/gui-docs/_images/visualizerplugin-06.png)

This repository contains the final code for the [Making Your Own Visualizer Plugin Tutorial](https://open-ephys.github.io/gui-docs/Tutorials/Making-Your-Own-Visualizer-Plugin.html) on the Open Ephys GUI documentation site.

If you get stuck following this tutorial, please [open an Issue](https://github.com/open-ephys-plugins/rate-viewer/issues) in this repository.


## Building from source

First, follow the instructions on [this page](https://open-ephys.github.io/gui-docs/Developer-Guide/Compiling-the-GUI.html) to build the Open Ephys GUI.

Then, clone this repository into a directory at the same level as the `plugin-GUI`, e.g.:
 
```
Code
├── plugin-GUI
│   ├── Build
│   ├── Source
│   └── ...
├── OEPlugins
│   └── rate-viewer
│       ├── Build
│       ├── Source
│       └── ...
```

### Windows

**Requirements:** [Visual Studio](https://visualstudio.microsoft.com/) and [CMake](https://cmake.org/install/)

From the `Build` directory, enter:

```bash
cmake -G "Visual Studio 17 2022" -A x64 ..
```

Next, launch Visual Studio and open the `OE_PLUGIN_rate-viewer.sln` file that was just created. Select the appropriate configuration (Debug/Release) and build the solution.

Selecting the `INSTALL` project and manually building it will copy the `.dll` and any other required files into the GUI's `plugins` directory. The next time you launch the GUI from Visual Studio, the Rate Viewer plugin should be available.


### Linux

**Requirements:** [CMake](https://cmake.org/install/)

From the `Build` directory, enter:

```bash
cmake -G "Unix Makefiles" ..
cd Debug
make -j
make install
```

This will build the plugin and copy the `.so` file into the GUI's `plugins` directory. The next time you launch the compiled version of the GUI, the Rate Viewer plugin should be available.


### macOS

**Requirements:** [Xcode](https://developer.apple.com/xcode/) and [CMake](https://cmake.org/install/)

From the `Build` directory, enter:

```bash
cmake -G "Xcode" ..
```

Next, launch Xcode and open the `rate-viewer.xcodeproj` file that now lives in the “Build” directory.

Running the `ALL_BUILD` scheme will compile the plugin; running the `INSTALL` scheme will install the `.bundle` file to `/Users/<username>/Library/Application Support/open-ephys/plugins-api8`. The Rate Viewer plugin should be available the next time you launch the GUI from Xcode.

**Note:** If you’re building the plugin on a Mac with Apple Silicon, you’ll need to make sure the `ALL_BUILD` scheme is set use “Rosetta”. You will likely need to first set the build target to “Any Mac,” and then select the “My Mac (Rosetta)” option that appears. It is possible to build a version of the GUI that runs natively on Apple Silicon, but there are a few extra steps involved, and it won’t work with plugins downloaded via the Plugin Installer. If you’re interested in this, please reach out to `support@open-ephys.org` for more info.

## Attribution

This plugin was originally developed by Anjal Doshi and Josh Siegle at the Allen Institute.
