project(
    'demo',
    'objc'
)

macos_frameworks = [
    'AppKit',
]

macos_frameworks_dep = dependency(
    'appleframeworks', 
    modules: macos_frameworks
)

sources = [
    'AppDelegate.m',
    'main.m',
]

executable(
    'demo',
    sources: sources,
    dependencies: macos_frameworks_dep
)
