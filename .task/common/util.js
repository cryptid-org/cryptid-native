const path = require('path');

function discoverAllComponents({ fg, paths }) {
    const names = fg.sync([`**/*${paths.test.sourceExtension}`], { cwd: paths.root })
        .map(p => path.basename(p, paths.test.sourceExtension));
    
    console.log(`Discovered ${names.length} components: ${names.join(' ')}`);

    return names;
};

function removeFiles({ paths, fg, fs }, globs) {
    console.log(`Removing files using globs: ${globs.join(', ')}`);

    const options = {
        absolute: true,
        cwd: paths.root
    };

    const files = fg.sync(globs, options);

    console.log(`\t${files.length} file(s) matched`);

    files
        .forEach(file => fs.unlinkSync(file));
};

function run({ spawnSync }, executable, args = [], opts = {}) {
    const result = spawnSync(executable, args, opts);
    
    if (result.status != 0) {
        throw result;
    }

    return result;
};

function version({ fs, paths }) {
    return fs.readFileSync(paths.version, { encoding: 'utf8'}).trim();
};

module.exports = {
    discoverAllComponents,
    removeFiles,
    run,
    version
};
