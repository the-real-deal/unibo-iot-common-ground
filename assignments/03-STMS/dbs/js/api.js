const API = {
    async getStatus() {
        try {
            const response = await fetch(`${CONFIG.CUS_BASE_URL}/status`);
            
            if (!response.ok) {
                throw new Error();
            }
            
            const data = await response.json();
            console.log('[CUS] getStatus:', data);
            return data;
            
        } catch (error) {
            console.error('[CUS] getStatus error:', error);
            // Ritorna stato NOT_AVAILABLE se non riesce a connettersi con il CUS
            return {
                mode: 'NOT_AVAILABLE'
            };
        }
    },

    async getWaterLevel() {
        try {
            const response = await fetch(
                `${CONFIG.CUS_BASE_URL}/water-level`
            );
            
            if (!response.ok) {
                throw new Error();
            }
            
            const data = await response.json();
            console.log('[CUS] getWaterLevels:', data, 'points');
            return data;
            
        } catch (error) {
            console.error('[CUS] getWaterLevels error:', error);
            return { levels: [], count: 0 };
        }
    },

    async getValveOpening() {
        try {
            const response = await fetch(`${CONFIG.CUS_BASE_URL}/valve-opening`);
            if (!response.ok) throw new Error();
            return await response.json();
        } catch (error) {
            console.error('[CUS] valve error:', error);
            return { opening: 0 };
        }
    },

    async setValveOpening(opening) {
        try {
            const response = await fetch(`${CONFIG.CUS_BASE_URL}/valve-opening`, {
                method: 'POST',
                body: JSON.stringify({
                    "opening" : opening
                })
            });
            
            if (!response.ok) {
                console.log(response);
                throw new Error();
            }
            
            const data = await response.json();
            console.log('[CUS] valve opening:', data);
            return data;
            
        } catch (error) {
            console.error('[CUS] valve error:', error);
            throw error;
        }
    },

    async setMode(newMode){
        try {
            const response = await fetch(`${CONFIG.CUS_BASE_URL}/status`, {
                method: 'POST',
                body: JSON.stringify({
                    "status" : newMode
                })
            });
            
            if (!response.ok) {
                console.log(response);
                throw new Error();
            }
            
            const data = await response.json();
            console.log('[CUS] current state/input mode:', data);
            return data;
            
        } catch (error) {
            console.error('[CUS] error:', error);
            throw error;
        }
    }
};