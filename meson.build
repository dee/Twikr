project('Twikr', 'cpp',default_options : ['cpp_std=c++11'])

# Documentation: https://mesonbuild.com/Qt5-module.html
qt5 = import('qt5')
qt5dep = dependency('qt5', modules : ['Core', 'Widgets'])
add_global_arguments('-Wno-ignored-attributes', language : 'cpp')
add_global_arguments('-Wno-nonportable-include-path', language : 'cpp')

generated_files = qt5.preprocess(
                            moc_headers : 'mainwidget.h',
                            ui_files : 'mainwidget.ui',
                            dependencies: [qt5dep])

# application icon
windows = import('windows')
res = windows.compile_resources('resources.rc')

executable('Twikr'
        , 'main.cpp'
        , 'mainwidget.cpp'
        , 'engine.cpp'
        , generated_files
        , res
        , dependencies : [qt5dep]
        , install : true)
