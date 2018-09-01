var dataManager = (function() {

    const maxSize = 60*24;
    var lightData = [];
    var moistData = [];
    var tempData = [];
    var humidData = [];

    function checkSize(arr) {
        if (arr.length > maxSize) {
            arr.shift();
        }
    }

    function pushToLight(data) {
        lightData.push(data);
        checkSize(lightData);
    }

    function pushToMoist(data) {
        moistData.push(data);
        checkSize(moistData);
    }

    function pushToTemp(data) {
        tempData.push(data);
        checkSize(tempData);
    }

    function pushToHumid(data) {
        humidData.push(data);
        checkSize(humidData);
    }

    function getLightData() {
        return lightData;
    }

    function getMoistData() {
        return moistData;
    }

    function getTempData() {
        return tempData;
    }

    function getHumidData() {
        return humidData;
    }

    return {
        pushToLight: pushToLight,
        pushToMoist: pushToMoist,
        pushToTemp: pushToTemp,
        pushToHumid: pushToHumid,
        getLightData: getLightData,
        getMoistData: getMoistData,
        getTempData: getTempData,
        getHumidData: getHumidData
    }
})();