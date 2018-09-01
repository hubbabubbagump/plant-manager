"use strict";

var socket = io.connect();

$(function() {
    const updateSocket = 'updateSetting';
    const waterSocket = 'waterPlant';
    const refreshSocket = 'refreshData';
    
    const lightId = $('#light');
    const moistId = $('#moist');
    const moistApproxId = $('#moist-approx');
    const tempId = $('#temp');
    const tempApproxId = $('#temp-approx');
    const humidId = $('#humid');
    const humidApproxId = $('#humid-approx');

    const lightValId = $('#light-thresh-val');
    const moistValId = $('#moist-thresh-val');
    const moistApxId = $('#moist-approx-val');
    const tempValId = $('#temp-thresh-val');
    const tempApxId = $('#temp-approx-val');
    const humidValId = $('#humid-thresh-val');
    const humidApxId = $('#humid-approx-val');

    const lightInfo = $('#light-info');
    const moistInfo = $('#moist-info');
    const tempInfo = $('#temp-info');
    const humidInfo = $('#humid-info');
    const conditionInfo = $('#condition');

    const updateId = $('#update');
    const updateData = $('#update-data');
    const waterPlant = $('#water-plant');

    //Create the 4 gauges

    //Used to update the values of all 4 gauages at once
    function updateValues(moisture, light, humidity, temp)
    {   moistGauge.update(moisture);
        lightGauge.update(light);
        humidGauge.redraw(humidity);
        tempGauge.redraw(temp);
    }

    function saveValues(){
        updateGaugeThreshhold();
    }

    //Create the 4 gauges
    var lightGauge = loadLiquidFillGauge("lightGauge", 50, lightConfig);
    var moistGauge = loadLiquidFillGauge("waterGauge", 50, moistConfig);
    createGauge("Temperature", "Temperature");
    createGauge("Humidity", "Humidity"); 
    

    updateData.click(function() {
        socket.emit(refreshSocket, 'update'); 
    });

    waterPlant.click(function() {
        socket.emit(waterSocket, 'waterPlant');
    });

    lightId.change(function() {
        lightValId.text($(this).val());
    });

    moistId.change(function() {
        moistValId.text($(this).val());
    });

    moistApproxId.change(function() {
        moistApxId.text($(this).val());
    });

    tempId.change(function() {
        tempValId.text($(this).val());
    });

    tempApproxId.change(function() {
        tempApxId.text($(this).val());
    });

    humidId.change(function() {
        humidValId.text($(this).val());
    });

    humidApproxId.change(function() {
        humidApxId.text($(this).val());
    });

    updateId.click(function() {
        var data = {};
        data.light = lightId.val();
        data.moist = moistId.val();
        data.moistApprox = moistApproxId.val();
        data.temp = tempId.val();
        data.tempApprox = tempApproxId.val();
        data.humid = humidId.val();
        data.humidApprox = humidApproxId.val();
        data = logicHelper.numberfyData(data);
        socket.emit(updateSocket, logicHelper.formatData(data));
    });

    socket.on(updateSocket, function(data) {
        var status = 'Good';
        var paramWarn = '';
        var plantInfo = logicHelper.getPlantInfo(data.plantData);
        saveValues();
        updateValues(plantInfo.moistVal, plantInfo.lightVal, plantInfo.humidVal, plantInfo.tempVal);
        updateLiquidThreshhold(plantInfo.moistStatus);
        lightInfo.text(plantInfo.lightThresh);
        moistInfo.text(plantInfo.moistRange);
        tempInfo.text(plantInfo.tempRange);
        humidInfo.text(plantInfo.humidRange);
        
        if (plantInfo.moistStatus == 'Warning') {
            status = 'Warning';
            paramWarn += 'Moisture';
        }
        if (plantInfo.tempStatus == 'Warning') {
            if (status == 'Warning') {
                paramWarn += ', ';
            }
            status = 'Warning';
            paramWarn += 'Temperature';
        }
        if (plantInfo.humidStatus == 'Warning') {
            if (status == 'Warning') {
                paramWarn += ', ';
            }
            status = 'Warning';
            paramWarn += 'Humidity'
        }
        
        conditionInfo.text(status + ': ' + paramWarn);
        if (data.addToGraph) {
            dataManager.pushToLight(plantInfo.lightVal);
            dataManager.pushToMoist(plantInfo.moistVal);
            dataManager.pushToTemp(plantInfo.tempVal);
            dataManager.pushToHumid(plantInfo.humidVal);
        }
    });
});