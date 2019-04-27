const { compileAllSources, compileExecutableForComponent } = require('./compile');
const { removeFiles, run } = require('./util');


function runMemoryCheck(dependencies, components) {
    try {
        compileAllSources(dependencies, []);

        const errors = testWithMemoryCheck(dependencies, components);

        if (errors.length > 1) {
            console.log(errors);
            throw new Error('There were errors during the memory check. Please see the log lines above.');
        }
    } finally {
        removeFiles(dependencies, ['*.o', '*.out']);
    }
};

function testWithMemoryCheck(dependencies, components) {
    const errors = [];

    for (const component of components) {
        console.log(`Running memory checks for ${component}`);

        const executable = compileExecutableForComponent(component, dependencies, []);

        const valgrindOptions = [
            '--leak-check=full',
            '--show-reachable=yes',
            '--error-exitcode=1',
            executable
        ]

        if (component == 'CryptID') {
            valgrindOptions.push('--', '--lowest-quick-check');
        }

        try {
            run(dependencies, 'valgrind', valgrindOptions, { cwd: dependencies.paths.root });

            removeFiles(dependencies, [executable]);
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

module.exports = {
    runMemoryCheck
};
