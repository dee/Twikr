project('Twikr', 'cpp',default_options : ['cpp_std=c++17'])

# Documentation: https://mesonbuild.com/Qt5-module.html
qt6 = import('qt6')
qt6dep = dependency('qt6', modules : ['Core', 'Widgets'])
add_global_arguments('-Wno-ignored-attributes', language : 'cpp')
add_global_arguments('-Wno-nonportable-include-path', language : 'cpp')

generated_files = qt6.preprocess(
                            moc_headers : ['mainwidget.h', 'engine.h', 'colorbutton.h'],
                            ui_files : 'mainwidget.ui',
                            dependencies: [qt6dep])

# application icon
windows = import('windows')
res = windows.compile_resources('resources.rc')

executable('Twikr'
        , 'main.cpp'
        , 'mainwidget.cpp'
        , 'engine.cpp'
        , 'colorbutton.cpp'
        , generated_files
        , res
        , dependencies : [qt6dep]
        , install : true)
