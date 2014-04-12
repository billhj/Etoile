ECHO ON
copy "..\lib\*d.dll" ".\plugins\"
copy "..\lib\*.dll" "..\Etoile-release\plugins\"
del  "..\Etoile-release\plugins\*d.dll"
del  "..\Etoile-release\plugins\QGLViewer2.dll"
del  "..\Etoile-release\plugins\QGLViewerd2.dll"
copy "*.exe" "..\Etoile-release\*.exe"
del  "..\Etoile-release\*d.exe"

copy  ".\graph\*.*" "..\Etoile-release\graph\"
copy  ".\img\*.*" "..\Etoile-release\img\"
copy  ".\shader\*.*" "..\Etoile-release\shader\"
copy  ".\styles\*.*" "..\Etoile-release\styles\"