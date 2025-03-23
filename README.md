
<h1 align="center">kraken_installer</h1>

###

<div align="center">
  <img height="350"  width="600" src="https://as1.ftcdn.net/jpg/05/61/65/40/1000_F_561654098_KDGDIAOIm6JToNF7ncaRzI61jLTA6GZt.webp" />
</div>

###

> [!WARNING]  
> Kraken is under development and not complete yet! Use at your own risk.
<div align="left">
  
</div>

**A Qt6-based GUI application** that simplifies kraken_os installation  by generating JSON configurations, extracting installation parameters, and running external install scripts.  

## 🖥️ Features  
- **Graphical User Interface (GUI)**: Built with **Qt6** for a modern and intuitive experience.  
- **JSON Configuration**: Dynamically generate and validate installation parameters.  
- **Script Integration**: Execute custom install scripts (Bash) stored in the [Kraken Installer TUI](https://github.com/n1cef/kraken_installer_tui) repository.  
- **Cross-Platform**: Works on Windows, Linux, and macOS (Qt6 compatibility).  





###

> [!NOTE]
 dependency:
 <h3><br></h3>
  cmake
   <h3><br></h3>

  qt6
 <h3> this app generate a json file and then  extract her content </h3>






###

<h1 align="left">Building Kraken_installer  from Source:</h1>

###

<h3 align="left">Follow the steps below to build and install Kraken from source:</h3>

### 1. Clone the repository
Clone the **Kraken installer ** repository from GitHub to your system:
```sh
git clone https://github.com/n1cef/kraken_installer_gui.git 

```
### 2. Navigate to the Kraken_installer  directory

```sh
cd kraken_installer_gui

```



### 3. Set up the build environment

```sh
mkdir build 


```

```sh
cd build  


```

```sh
cmake .. 


```

### 4.  Build Kraken

```sh
 cmake --build .


```
### 5. Install Kraken

```sh
 sudo cp kraken /usr/local/bin/krakeninsatll


```
```sh
 sudo ln -sf /usr/local/bin/krakeninstall /usr/bin/krakeninstall


```
```sh
 sudo chmod +x /usr/bin/krakeninstall


```
### 6. create desktop entry 
```sh
 sudo cp krakeninstall.desktop /usr/share/applications/


```
```sh
sudo cp krakeninstall.png /usr/share/icons/hicolor/48x48/apps/
```
```sh
sudo gtk-update-icon-cache /usr/share/icons/hicolor/
```


