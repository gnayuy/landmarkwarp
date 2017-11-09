# landmarkwarp
3D Image warp with landmarks specified

## INSTALL

Require [ITK][] to compile this.

### build ITK
```
git clone https://github.com/InsightSoftwareConsortium/ITK.git
cd ITK
mkdir build
cd build
ccmake ..
make -j8
sudo make install
```
### build landmarkwarp
```
mkdir build
cd build
ccmake ../src
make -j8
```
## Usage
```
LandmarkWarping3.app/Contents/MacOS/LandmarkWarping3 landmarksFile fixedImage movingImage deformedMovingImage
```
* landmarksFile is saved as a list of landmarks "target_x, target_y, target_z, source_x, source_y, source_z"


[ITK]: https://github.com/InsightSoftwareConsortium/ITK.git
