import qbs

CppApplication {
    type: "application" // To suppress bundle generation on Mac
    consoleApplication: true
    files: [ "SceneGraph.cpp", "SceneGraph.h",
        "CollisionClass.cpp", "CollisionClass.h",
        "BackdropClass.cpp", "BackdropClass.h",
        "AnimationNodes.cpp", "AnimationNodes.h",
        "ObjectNodes.cpp", "ObjectNodes.h", "propertyStructures.h",
        "CameraClass.cpp", "CameraClass.h",
        "ObjectPickerClass.cpp", "ObjectPickerClass.h",
        "DrawingClass.cpp", "DrawingClass.h",
        "main.cpp" , "initialise.cpp", "initialise.h",
        "ObjLoaderClass.cpp", "ObjLoaderClass.h",
        "lodepng.cpp", "TGALoader.cpp"]
    cpp.commonCompilerFlags: [ "-lglut -lGL -lGLU" ]
    cpp.staticLibraries: [ "glut", "GL", "GLU" ]
}

