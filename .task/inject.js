const path = require('path');

const paths = (function setupPaths() {
    const root = path.resolve(__dirname, '..');
    const version = path.join(root, 'VERSION');
    const cryptid = {
        sourceDir: path.join(root, 'src'),
        sourceExtension: '.c',
        includeDir: path.join(root, 'include')
    };
    const dependenciesRoot = path.join(root, 'dependencies');
    const dependencies = {
        root,
        gmp: {
            includeDir: path.join(dependenciesRoot, 'gmp', 'include')
        },
        greatest: {
            includeDir: path.join(dependenciesRoot, 'greatest', 'include'),
            miscDir: path.join(dependenciesRoot, 'greatest', 'misc')
        },
        sha: {
            includeDir: path.join(dependenciesRoot, 'sha', 'include'),
            sourceDir: path.join(dependenciesRoot, 'sha', 'src')
        }
    };
    const test = {
        sourceDir: path.join(root, 'test', 'src'),
        sourceExtension: '.test.c',
        output(component) {
            return `${component}.out`;
        },
    };
    const coverage = {
        lcovOutput: path.join(root, 'cryptid.info'),
        genhtmlOutput: path.join(root, 'coverage')
    }
    test.componentSourceFile = function componentSourceFile(componentName) {
        return path.join(test.sourceDir, `${componentName}.test.c`);
    };

    const build = {
        outputDirectory: path.join(root, 'build')
    };
    build.libraryFile = function libraryFile(version) {
        return path.join(build.outputDirectory, `cryptid-${version}.a`);
    };

    return {
        root,
        version,
        cryptid,
        dependencies,
        test,
        coverage,
        build
    };
})();

module.exports = function inject() {
    return {
        paths
    };
};
