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
        },
        tinyregexc: {
            includeDir: path.join(dependenciesRoot, 'tiny-regex-c', 'include'),
            sourceDir: path.join(dependenciesRoot, 'tiny-regex-c', 'src')
        }
    };
    const testResultsDirectory = path.join(root, 'test-results');
    const test = {
        sourceDir: path.join(root, 'test', 'src'),
        sourceExtension: '.test.c',
        output(component) {
            return `${component}.out`;
        },
        resultsDirectory: testResultsDirectory,
        tapify: path.join(dependencies.greatest.miscDir, 'entapment.awk')
    };
    const coverageRoot = path.join(root, 'coverage');
    const coverage = {
        root: coverageRoot,
        lcovOutputDir: path.join(coverageRoot, 'lcov'),
        lcovOutputFile: path.join(coverageRoot, 'lcov', 'cryptid.info'),
        genhtmlOutputDir: path.join(coverageRoot, 'html')
    };
    test.componentSourceFile = function componentSourceFile(componentName) {
        return path.join(test.sourceDir, `${componentName}.test.c`);
    };

    const build = {
        outputDirectory: path.join(root, 'build')
    };
    build.libraryFile = function libraryFile() {
        return path.join(build.outputDirectory, `libcryptid.a`);
    };

    const memcheckRoot = path.join(root, 'memcheck');
    const memcheck = {
        root: memcheckRoot
    };

    memcheck.componentMemcheckFile = function componentMemcheckFile(componentName) {
        return path.join(memcheck.root, `${componentName}.memcheck.xml`);
    };

    return {
        root,
        version,
        cryptid,
        dependencies,
        test,
        coverage,
        build,
        memcheck
    };
})();

module.exports = function inject() {
    return {
        paths
    };
};
