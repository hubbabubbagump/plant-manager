"use strict";

var logicHelper = (function() {

    function checkMax(original, max) {
        if (original > max) {
            return max;
        }
        return original;
    }

    function checkMin(original, min) {
        if (original < min) {
            return min;
        }
        return original;
    }

    function checkThreshold(val, upper, lower) {
        var status;
        if (val < lower || val > upper) {
            status = 'Warning';
        } else {
            status = 'Good';
        }
        return status;

    }
    
    function formatData(data) {
        const max = 100;
        const min = 0;
        const tempMax = 80;
        const tempMin = -40;
        var dataPkg = {};
        dataPkg.light = data.light;
        dataPkg.moistUpper = checkMax(data.moist + data.moistApprox, max);
        dataPkg.moistLower = checkMin(data.moist - data.moistApprox, min);
        dataPkg.tempUpper = checkMax(data.temp + data.tempApprox, tempMax);
        dataPkg.tempLower = checkMin(data.temp - data.tempApprox, tempMin);
        dataPkg.humidUpper = checkMax(data.humid + data.humidApprox, max);
        dataPkg.humidLower = checkMin(data.humid - data.humidApprox, min);
        return dataPkg;
    }

    function numberfyData(data) {
        var dataNmb = {};
        for (var entry in data) {
            if (!data.hasOwnProperty(entry)) {
                continue;
            }
            dataNmb[entry] = Number(data[entry]);
        }
        return dataNmb;
    }

    function getPlantInfo(data) {
        var dataArray = data.split(',');
        const lightThresh = Number(dataArray[0]);
        const waterThreshUpper = Number(dataArray[1]);
        const waterThreshLower = Number(dataArray[2]);
        const tempThreshUpper = Number(dataArray[3]);
        const tempThreshLower = Number(dataArray[4]);
        const humidThreshUpper = Number(dataArray[5]);
        const humidThreshLower = Number(dataArray[6]);
        const lightVal = Number(dataArray[7]);
        const moistVal = Number(dataArray[8]);
        const tempVal = Number(dataArray[9]);
        const humidVal = Number(dataArray[10]);
        var plantInfo = {};
        plantInfo.lightThresh = lightThresh;
        plantInfo.moistRange = '[' + waterThreshLower + '-' + waterThreshUpper + ']';
        plantInfo.tempRange = '[' + tempThreshLower + '-' + tempThreshUpper + ']';
        plantInfo.humidRange = '[' + humidThreshLower + '-' + humidThreshUpper + ']';
        plantInfo.lightVal = lightVal;
        plantInfo.moistVal = moistVal;
        plantInfo.tempVal = tempVal;
        plantInfo.humidVal = humidVal;
        plantInfo.moistStatus = checkThreshold(moistVal, waterThreshUpper, waterThreshLower);
        plantInfo.tempStatus = checkThreshold(tempVal, tempThreshUpper, tempThreshLower);
        plantInfo.humidStatus = checkThreshold(humidVal, humidThreshUpper, humidThreshLower);
        console.log(humidVal + '+' + humidThreshUpper + '+' + humidThreshLower)
        return plantInfo;
    }

    function appendPlantInfo(tagId, typeParam, reading, thresh, status) {
        tagId.append('Current ' + typeParam + ' Reading: ' + reading + '<br>');
        tagId.append('Current ' + typeParam + ' Threshold: ' + thresh + '<br>');
        tagId.append('Current ' + typeParam + ' Status: ' + status + '<br>');
    }

    return {
        formatData: formatData,
        numberfyData: numberfyData,
        getPlantInfo: getPlantInfo,
        appendPlantInfo: appendPlantInfo
    }
})();