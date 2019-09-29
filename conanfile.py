from conans import ConanFile, CMake

class BackupConan(ConanFile):
    settings = ('os', 'compiler', 'build_type', 'arch')
    requires = (
        'OpenSSL/1.0.2s@conan/stable',
        'sqlitecpp/2.2.0@bincrafters/stable',
        'date/2.4.1@bincrafters/stable'
    )
    generators = 'cmake'

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy('*.dll', src='bin', dst='bin')
        self.copy('*.dylib*', src='lib', dst='lib')
        self.copy('*.so', src='lib', dst='lib')
