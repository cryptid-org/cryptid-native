const { run } = require('./util');


function compileAllSources({ klawSync, paths, spawnSync }, extraArguments = []) {
    const cryptidSourceFiles = walkDirectory(klawSync, paths.cryptid.sourceDir, paths.cryptid.sourceExtension);
    const shaSourceFiles = walkDirectory(klawSync, paths.dependencies.sha.sourceDir, paths.cryptid.sourceExtension);
    const tinyregexcSourceFiles = walkDirectory(klawSync, paths.dependencies.tinyregexc.sourceDir, paths.cryptid.sourceExtension);

    const opts = [
        ...cryptidSourceFiles,
        ...shaSourceFiles,
        ...tinyregexcSourceFiles,
        `-I${paths.cryptid.includeDir}`,
        `-I${paths.dependencies.gmp.includeDir}`,
        `-I${paths.dependencies.sha.includeDir}`,
        `-I${paths.dependencies.tinyregexc.includeDir}`,
        '-D__CRYPTID_GMP',
        '-D__CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION',
        '-D__CRYPTID_HESS_IDENTITY_BASED_SIGNATURE',
        '-std=c99',
        '-Wall',
        '-Wextra',
        '-Werror',
        '-c'
    ];

    opts.push(...extraArguments)

    compile({ spawnSync, paths }, opts);
};

function compileExecutableForComponent(componentName, { klawSync, fs, paths, spawnSync }, extraArguments = []) {
    const objectFiles = walkDirectory(klawSync, paths.root, '.o');
    
    const componentSourceFile = paths.test.componentSourceFile(componentName);

    const testExecutable = paths.test.output(componentSourceFile);

    if (!fs.pathExistsSync(componentSourceFile)) {
        throw new Error(`Component ${componentSourceFile} does not exist!`);
    }

    const opts = [
        ...objectFiles,
        componentSourceFile,
        `-I${paths.cryptid.includeDir}`,
        `-I${paths.dependencies.gmp.includeDir}`,
        `-I${paths.dependencies.greatest.includeDir}`,
        `-I${paths.dependencies.sha.includeDir}`,
        `-I${paths.dependencies.tinyregexc.includeDir}`,
        '-D__CRYPTID_GMP',
        '-D__CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION',
        '-D__CRYPTID_HESS_IDENTITY_BASED_SIGNATURE',
        '-g',
        '-std=c99',
        '-o', testExecutable,
        '-lm',
        '-lgmp'
    ];
    
    opts.push(...extraArguments)

    compile({ spawnSync, paths }, opts);

    return testExecutable;
};

function compile(dependencies, opts) {
    run(dependencies, 'gcc', opts, { cwd: dependencies.paths.root });
};

function walkDirectory(klawSync, baseDirectory, extension) {
    return klawSync(baseDirectory, { nodir: true })
        .map(p => p.path)
        .filter(p => p.endsWith(extension))
};

module.exports = {
    compileAllSources,
    compileExecutableForComponent
};
