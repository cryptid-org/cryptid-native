(function randomIIFE() {
    function cryptoRandom(buffer, byteCount) {
        const FAILURE = 1;
        const SUCCESS = 0;
        const isNode = (typeof window == 'undefined') && require;
        const hasCryptoWithGetRandomValues = (typeof crypto != 'undefined') && crypto.getRandomValues;

        if (isNode) {
            return cryptoRandomNode(buffer, byteCount);
        } else if (hasCryptoWithGetRandomValues) {
            return cryptoRandomBrowser(buffer, byteCount);
        } else {
            return FAILURE;
        }

        function cryptoRandomNode(buffer, byteCount) {
            const tempArray = new Uint8Array(byteCount);
    
            try {
                require('crypto').randomFillSync(tempArray);
            } catch(e) {
                return FAILURE;
            }
    
            Module.writeArrayToMemory(tempArray, buffer);
    
            return SUCCESS;
        }
    
        function cryptoRandomBrowser(buffer, byteCount) {
            const tempArray = new Uint8Array(byteCount);
    
            try {
                crypto.getRandomValues(tempArray);
            } catch(e) {
                return FAILURE;
            }
    
            Module.writeArrayToMemory(tempArray, buffer);
    
            return SUCCESS;
        }
    }

    mergeInto(LibraryManager.library, {
        cryptoRandom
    });
})();
