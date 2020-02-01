const { compileAllSources, compileExecutableForComponent } = require('./compile');
const { runTests } = require('./test');
const { removeFiles, run } = require('./util');


const coverageCompilationArguments = ['-fprofile-arcs', '-ftest-coverage'];
const excludedFromCoverage = ['*.test.gcda', '*.test.gcno', 'sha.gcno', 'sha.gcda'];
const gcovOutputGlobs = ['*.gcda', '*.gcno'];

function generateHtmlCoverage(dependencies, components) {
    dependencies.fs.removeSync(dependencies.paths.coverage.root);

    generateGcovCoverage(dependencies, components);

    generateLcovCoverage(dependencies);

    const args = [
        '-o',
        dependencies.paths.coverage.genhtmlOutputDir,
        dependencies.paths.coverage.lcovOutputFile
    ];

    run(dependencies, 'genhtml', args, { cwd: dependencies.paths.root });
};

function generateGcovCoverage(dependencies, components) {
    try {
        runTests(dependencies, components, coverageCompilationArguments);

        removeFiles(dependencies, excludedFromCoverage);
    } catch (e) {
        // Upon failure, we don't want partial results.
        removeFiles(dependencies, gcovOutputGlobs);

        throw e;
    } finally {
        removeFiles(dependencies, ['*.o', '*.out']);
    }
};

function testComponentsWithCoverage(dependencies, components) {
    const errors = [];

    compileAllSources(dependencies, ['-g', ...coverageCompilationArguments]);

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
};

function generateLcovCoverage(dependencies) {
    try {
        dependencies.fs.ensureDirSync(dependencies.paths.coverage.lcovOutputDir)

        const args = [
            '--directory', '.',
            '--capture',
            '--output-file', dependencies.paths.coverage.lcovOutputFile
        ];

        run(dependencies, 'lcov', args, { cwd: dependencies.paths.root });
    } finally {
        removeFiles(dependencies, gcovOutputGlobs);
    }
};

function reportCoverage({ fs, paths, reportToCoveralls }) {
    const lcov = fs.readFileSync(paths.coverage.lcovOutputFile).toString();

    const result = reportToCoveralls(lcov);

    console.log(result);
};

module.exports = {
    runTestsWithCoverage: generateHtmlCoverage,
    reportCoverage
};
