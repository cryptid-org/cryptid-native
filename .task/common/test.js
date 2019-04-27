const { compileAllSources, compileExecutableForComponent } = require('./compile');
const { removeFiles, run } = require('./util');

function runTests(dependencies, components) {
    try {
        const errors = testComponents(dependencies, components);

        if (errors.length > 0) {
            console.log(errors);
            throw new Error('There were errors running the tests. Please see the log lines above.');
        }
    } finally {
        removeFiles(dependencies, ['*.o', '*.out']);
    }
};

function testComponents(dependencies, components) {
    const errors = [];

    compileAllSources(dependencies, []);

    for (const component of components) {
        console.log(`Testing ${component}`);

        const executable = compileExecutableForComponent(component, dependencies, []);

        try {
            run(dependencies, executable, ['-v']);
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

module.exports = {
    runTests
};
