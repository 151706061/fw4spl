
set( NAME Ex01VolumeRendering )
set( VERSION 0.1 )
set( TYPE APP )
set( DEPENDENCIES  )
set( REQUIREMENTS
    appXml2
    ctrlCamp
    ctrlSelection
    dataReg
    fwlauncher
    gui
    guiQt
    imageConfig
    io
    ioAtoms
    ioVTK
    preferences
    scene2D
    servicesReg
    uiIO
    uiImageQt
    uiTF
    visuVTK
    visuVTKAdaptor
    visuVTKQt
    visuVTKVRAdaptor
)

bundleParam(appXml2 PARAM_LIST config PARAM_VALUES Ex01VolumeRendering)
