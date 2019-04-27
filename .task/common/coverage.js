const { compileAllSources, compileExecutableForComponent } = require('./compile');
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
        const errors = testComponentsWithCoverage(dependencies, components);

        if (errors.length > 0) {
            console.log(errors);
            throw new Error('There were errors during the coverage calculation. Please see the log lines above.');
        }

        removeFiles(dependencies, excludedFromCoverage);
    } catch (e) {
        // Upon failure, we don't want partial results.
        removeFiles(dependencies, gcovOutputGlobs);

        throw e;
    } finally {
        removeFiles(dependencies, ['*.o', '*.out']);
    }
}

function testComponentsWithCoverage(dependencies, components) {
    const errors = [];

    compileAllSources(dependencies, coverageCompilationArguments);

    for (const component of components) {
        console.log(`Collecting test coverage for ${component}`);

        const executable = compileExecutableForComponent(component, dependencies, coverageCompilationArguments);
        try {
            run(dependencies, executable);
        } catch (e) {
            errors.push({
                component,
                error: e
            });
        } finally {
            removeFiles(dependencies, [executable]);
        }

        console.log('\n');
    }

    return errors;
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
