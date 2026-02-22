foundation_sources = [
    'EnableWeakPtrTest.cpp',
    'RefCountedTest.cpp',
    'RefPtrTest.cpp',
]

foundation = executable(
    'foundation',
    foundation_sources,
    dependencies: tests_deps,
    install: false,
)

test('Foundation', foundation)
