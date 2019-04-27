const { testComponents } = require('./test');
const { removeFiles, run } = require('./util');


const coverageCompilationArguments = ['-fprofile-arcs', '-ftest-coverage'];
const excludedFromCoverage = ['*.test.gcda', '*.test.gcno', 'sha.gcno', 'sha.gcda'];
const gcovOutputGlobs = ['*.gcda', '*.gcno'];

function generateHtmlCoverage(dependencies, components) {
    generateGcovCoverage(dependencies, components);

    try {
        generateLcovCoverage(dependencies);

        const args = [
            '-o',
            dependencies.paths.coverage.genhtmlOutput,
            dependencies.paths.coverage.lcovOutput
        ];

        run(dependencies, 'genhtml', args, { cwd: dependencies.paths.root });
    } finally {
        removeFiles(dependencies, [dependencies.paths.coverage.lcovOutput]);
    }
}

function generateGcovCoverage(dependencies, components) {
    try {
        testComponents(dependencies, components, coverageCompilationArguments);

        removeFiles(dependencies, excludedFromCoverage);
    } catch (e) {
        // Upon failure, we don't want partial results.
        removeFiles(dependencies, gcovOutputGlobs);

        throw e;
    } finally {
        removeFiles(dependencies, ['*.o', '*.out']);
    }
}

function generateLcovCoverage(dependencies) {
    const args = [
        '--directory', '.',
        '--capture',
        '--output-file', dependencies.paths.coverage.lcovOutput
    ];

    run(dependencies, 'lcov', args, { cwd: dependencies.paths.root });

    removeFiles(dependencies, gcovOutputGlobs);
}

module.exports = {
    generateGcovCoverage,
    generateHtmlCoverage
};
