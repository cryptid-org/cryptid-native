const path = require('path');

const paths = (function setupPaths() {
    const root = path.resolve(__dirname, '..');
    const cryptid = {
        sourceDir: path.join(root, 'src'),
        sourceExtension: '.c',
        includeDir: path.join(root, 'include')
    };
    const thirdParty = {
        sourceDir: path.join(root, 'third-party', 'src'),
        includeDir: path.join(root, 'third-party', 'include')
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

    return {
        root,
        cryptid,
        thirdParty,
        test,
        coverage
    };
})();

module.exports = function inject() {
    return {
        paths
    };
};
