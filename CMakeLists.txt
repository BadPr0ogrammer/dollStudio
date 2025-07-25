
cmake_minimum_required(VERSION 3.16)

set(MYNAME dollstudio)

project(${MYNAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC OFF)

set(CMAKE_GLOBAL_AUTOGEN_TARGET OFF)
set(AUTOGEN_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/autogen")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)
find_package(VTK REQUIRED)
find_package(assimp REQUIRED)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(${MYNAME}
    MANUAL_FINALIZATION
    app.cpp 
    app.h 
    main.cpp 
    manager.cpp 
    manager.h 
    vtkitem.cpp 
    vtkitem.h
    qml.qrc

    f3d/animationManager.cxx
    f3d/animationManager.h
    f3d/init.cxx
    f3d/init.h
    f3d/window_impl.cxx
    f3d/window_impl.h
    f3d/options.cxx
    f3d/options.h
    f3d/scene_impl.cxx
    f3d/scene_impl.h
    f3d/camera_impl.cxx
    f3d/camera_impl.h
    f3d/image.cxx
    f3d/image.h
    f3d/context.cxx
    f3d/context.h
    f3d/factory.cxx
    f3d/factory.h
    f3d/utils.cxx
    f3d/utils.h
    f3d/levenshtein.cxx
    f3d/levenshtein.h
    f3d/plugin.cxx
    f3d/plugin.h

    f3d/F3D/F3DColoringInfoHandler.cxx
    f3d/F3D/F3DColoringInfoHandler.h
    f3d/F3D/F3DLog.cxx
    f3d/F3D/F3DLog.h
    f3d/F3D/F3DDefaultHDRI.cxx
    f3d/F3D/F3DDefaultHDRI.h
    f3d/F3D/F3DUtils.cxx
    f3d/F3D/F3DUtils.h

    f3d/vtk/vtkF3DMetaImporter.cxx
    f3d/vtk/vtkF3DMetaImporter.h
    f3d/vtk/vtkF3DImporter.cxx
    f3d/vtk/vtkF3DImporter.h
    f3d/vtk/vtkF3DConsoleOutputWindow.cxx
    f3d/vtk/vtkF3DConsoleOutputWindow.h
    f3d/vtk/vtkF3DGenericImporter.cxx
    f3d/vtk/vtkF3DGenericImporter.h
    f3d/vtk/vtkF3DRenderer.cxx
    f3d/vtk/vtkF3DRenderer.h
    f3d/vtk/vtkF3DPostProcessFilter.cxx
    f3d/vtk/vtkF3DPostProcessFilter.h
    f3d/vtk/vtkF3DCachedLUTTexture.cxx
    f3d/vtk/vtkF3DCachedLUTTexture.h
    f3d/vtk/vtkF3DOpenGLGridMapper.cxx
    f3d/vtk/vtkF3DOpenGLGridMapper.h
    f3d/vtk/vtkF3DOverlayRenderPass.cxx
    f3d/vtk/vtkF3DOverlayRenderPass.h
    f3d/vtk/vtkF3DPolyDataMapper.cxx
    f3d/vtk/vtkF3DPolyDataMapper.h
    f3d/vtk/vtkF3DRenderPass.cxx
    f3d/vtk/vtkF3DRenderPass.h
    f3d/vtk/vtkF3DSolidBackgroundPass.cxx
    f3d/vtk/vtkF3DSolidBackgroundPass.h
    f3d/vtk/vtkF3DUserRenderPass.cxx
    f3d/vtk/vtkF3DUserRenderPass.h
    f3d/vtk/vtkF3DHexagonalBokehBlurPass.cxx
    f3d/vtk/vtkF3DHexagonalBokehBlurPass.h
    f3d/vtk/vtkF3DExternalRenderWindow.cxx
    f3d/vtk/vtkF3DExternalRenderWindow.h
    f3d/vtk/vtkF3DInteractorEventRecorder.cxx
    f3d/vtk/vtkF3DInteractorEventRecorder.h
    f3d/vtk/vtkF3DInteractorStyle.cxx
    f3d/vtk/vtkF3DInteractorStyle.h
    f3d/vtk/vtkF3DNoRenderWindow.cxx
    f3d/vtk/vtkF3DNoRenderWindow.h
    f3d/vtk/vtkF3DWGLRenderWindow.cxx
    f3d/vtk/vtkF3DWGLRenderWindow.h
    f3d/vtk/vtkF3DObjectFactory.cxx
    f3d/vtk/vtkF3DObjectFactory.h
    f3d/vtk/vtkF3DPointSplatMapper.cxx
    f3d/vtk/vtkF3DPointSplatMapper.h
    f3d/vtk/vtkF3DBitonicSort.cxx
    f3d/vtk/vtkF3DBitonicSort.h
    f3d/vtk/vtkF3DBitonicSortFunctions.cxx
    f3d/vtk/vtkF3DBitonicSortFunctions.h
    f3d/vtk/vtkF3DBitonicSortGlobalDisperseCS.cxx
    f3d/vtk/vtkF3DBitonicSortGlobalDisperseCS.h
    f3d/vtk/vtkF3DBitonicSortGlobalFlipCS.cxx
    f3d/vtk/vtkF3DBitonicSortGlobalFlipCS.h
    f3d/vtk/vtkF3DBitonicSortLocalDisperseCS.cxx
    f3d/vtk/vtkF3DBitonicSortLocalDisperseCS.h
    f3d/vtk/vtkF3DBitonicSortLocalSortCS.cxx
    f3d/vtk/vtkF3DBitonicSortLocalSortCS.h
    f3d/vtk/vtkF3DComputeDepthCS.cxx
    f3d/vtk/vtkF3DComputeDepthCS.h
    f3d/vtk/vtkF3DMemoryMesh.cxx
    f3d/vtk/vtkF3DMemoryMesh.h
    f3d/vtk/vtkF3DAssimpImporter.cxx
    f3d/vtk/vtkF3DAssimpImporter.h
    f3d/vtk/vtkF3DCachedSpecularTexture.cxx
    f3d/vtk/vtkF3DCachedSpecularTexture.h
)

qt_add_qml_module(${MYNAME}
    URI Dollstudio
    VERSION 1.0
    QML_FILES
        main.qml 
	DSqml/AboutDialog.qml
)

if (WIN32)
target_link_libraries(${MYNAME}
    PRIVATE
    Qt6::Quick Qt6::Qml Qt6::Gui Qt6::Core
    Qt6::QuickPrivate
    ${VTK_LIBRARIES}
    assimp::assimp
    assimp::zlibstatic
    #Qt6::qtquickdialogs2quickimplplugin
    #Qt6::qtquickdialogsplugin
    C:/Qt/6.9.1/msvc2022_64/lib/Qt6QuickDialogs2d.lib
    C:/Qt/6.9.1/msvc2022_64/lib/Qt6QuickDialogs2QuickImpld.lib
    Dwmapi.lib
)
else()
target_link_libraries(${MYNAME}
    PRIVATE
    Qt6::Quick Qt6::Qml Qt6::Gui Qt6::Core
    ${VTK_LIBRARIES}
    assimp::assimp
    assimp::zlibstatic
    Qsk::QSkinny
    Qsk::QmlExport
    #/usr/local/lib64/qskinny/libqsktestsupport.so
    Qt6::qtquickdialogs2quickimplplugin
    Qt6::qtquickdialogsplugin
    #/home/user/Qt/6.9.1/gcc_64/lib/libQt6QuickDialogs2.so 
)
endif()

target_include_directories(${MYNAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/f3d
    ${CMAKE_SOURCE_DIR}/f3d/F3D
    ${CMAKE_SOURCE_DIR}/f3d/vtk
)
target_include_directories(${MYNAME} PUBLIC ${ASSIMP_ROOT_DIR}/include)

if (WIN32)
set(QTINCL C:/Qt/6.9.1/msvc2022_64/include)
else()
set(QTINCL /home/user/Qt/6.9.1/gcc_64/include)
endif()

target_include_directories(${MYNAME} PUBLIC
    ${QTINCL}/QtQuickDialogs2Utils/6.9.1
    ${QTINCL}/QtQuickDialogs2/6.9.1
    ${QTINCL}/QtQuickDialogs2Utils/6.9.1/QtQuickDialogs2Utils
    ${QTINCL}/QtQuickDialogs2Utils/6.9.1/QtQuickDialogs2Utils/private
    ${QTINCL}/QtQuickDialogs2/6.9.1/QtQuickDialogs2
    ${QTINCL}/QtQuickDialogs2/6.9.1/QtQuickDialogs2/private
)

vtk_module_autoinit(TARGETS ${MYNAME} MODULES ${VTK_LIBRARIES})

include(GNUInstallDirs)
install(TARGETS ${MYNAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
