//This code obtained from http://bl.ocks.org/tomerd/1499279

/*This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org>
*/

function updateGaugeThreshhold(){
    /* Updates the temperature Guage */
    var min, max;
    var tempObject = document.getElementById("temp");
    var tempPercent = (Number(tempObject.value)+40)/120;
    var tempThreshObject = document.getElementById("temp-approx");
    var tempThreshPercent = Number(tempThreshObject.value)/100;
    var defaultOffset = 0.1;
    var tempConfig = 
    {

        size: 500,
        min: undefined != min ? min : -40,
        max: undefined != max ? max : 80,
        minorTicks: 5
    }
    var range = tempConfig.max - tempConfig.min;
    var goodZone = [];
    var okZone = [];
    var badZone = [];

    goodZone.min = tempConfig.min + range*(tempPercent-tempThreshPercent-defaultOffset);
    goodZone.max = tempConfig.min + range*(tempPercent+tempThreshPercent+defaultOffset);
    okZone.min = goodZone.min-(range*defaultOffset);
    okZone.max = goodZone.max+(range*defaultOffset);
    badZone.min = tempConfig.min+okZone.min;
    badZone.max = tempConfig.max-okZone.max;
    tempConfig.greenZones = [{from:goodZone.min, to:goodZone.max}];
    tempConfig.yellowZones = [{ from: okZone.min, to: goodZone.min},{ from: goodZone.max, to: okZone.max}];
    tempConfig.redZones = [{ from: tempConfig.min, to: okZone.min},{ from:okZone.max , to: tempConfig.max}];
    tempGauge = new Gauge("tempGauge", tempConfig);
    tempGauge.render();


    /* Updates the humidity Guage */
    var humidConfig = 
    {
        size: 500,
        min: undefined != min ? min : 0,
        max: undefined != max ? max : 100,
        minorTicks: 10
    }
    
    var humidObject = document.getElementById("humid");
    var humidPercent = (Number(humidObject.value))/100;
    var humidThreshObject = document.getElementById("humid-approx");
    var humidThreshPercent = Number(humidThreshObject.value)/100;
    var defaultHumidOffset = 0.1;
    var greenMin;
    if( (humidPercent-humidThreshPercent-defaultHumidOffset)*100 < 0){
        greenMin = 0;
    }
    else{
        greenMin = (humidPercent-humidThreshPercent-defaultHumidOffset)*100;
    }

    var greenMax;
    if( (humidPercent+humidThreshPercent+defaultHumidOffset)*100 > 100){
        greenMax = 100;
    }
    else{
        greenMax = (humidPercent+humidThreshPercent+defaultHumidOffset)*100;
    }

    var range = humidConfig.max - humidConfig.min;
    humidConfig.blueZones = [{from:(humidPercent+humidThreshPercent+defaultHumidOffset)*100, to:humidConfig.max}]
    humidConfig.greenZones = [{ from: greenMin , to: greenMax }];
    humidConfig.redZones = [{ from: humidConfig.min, to: (humidPercent-defaultHumidOffset-humidThreshPercent)*100 }];
    humidGauge = new Gauge("humidGauge", humidConfig);
    humidGauge.render();
    
}
//Creates the humidity and temperature gauges        
function createGauge(name, label, min, max)
{
    var humidConfig = 
    {
        size: 500,
        min: undefined != min ? min : 0,
        max: undefined != max ? max : 100,
        minorTicks: 10
    }
    
    var range = humidConfig.max - humidConfig.min;
    humidConfig.blueZones = [{from:humidConfig.min + range*0.7, to:humidConfig.max}]
    humidConfig.greenZones = [{ from: humidConfig.min + range*0.4, to: humidConfig.min + range*0.7 }];
    humidConfig.redZones = [{ from: humidConfig.min, to: humidConfig.min + range*0.4 }];
    humidGauge = new Gauge("humidGauge", humidConfig);
    humidGauge.render();

    var tempConfig = 
    {
        size: 500,
        min: undefined != min ? min : -40,
        max: undefined != max ? max : 80,
        minorTicks: 5
    }
    
    var range = tempConfig.max - tempConfig.min;
    tempConfig.greenZones = [{from:tempConfig.min  + range*0.3, to:tempConfig.min + range*0.7}]
    tempConfig.yellowZones = [{ from: tempConfig.min + range*0.7, to: tempConfig.min + range*0.9 }, { from: tempConfig.min + range*0.1, to: tempConfig.min + range*0.3 }];
    tempConfig.redZones = [{ from: tempConfig.min + range*0.9, to: tempConfig.max },{ from: tempConfig.min , to: tempConfig.min + range*0.1} ];
    tempGauge = new Gauge("tempGauge", tempConfig);
    tempGauge.render();
}

function Gauge(placeholderName, configuration)
{
    this.placeholderName = placeholderName;
    
    var self = this; // for internal d3 functions
    
    this.configure = function(configuration)
    {
        this.config = configuration;
        
        this.config.size = this.config.size * 0.9;
        
        this.config.raduis = this.config.size * 0.97 / 2;
        this.config.cx = this.config.size / 2;
        this.config.cy = this.config.size / 2;
        
        this.config.min = undefined != configuration.min ? configuration.min : 0; 
        this.config.max = undefined != configuration.max ? configuration.max : 100; 
        this.config.range = this.config.max - this.config.min;
        
        this.config.majorTicks = configuration.majorTicks || 5;
        this.config.minorTicks = configuration.minorTicks || 2;
        
        this.config.blueColor  = configuration.blueColor || "#0000FF";
        this.config.greenColor  = configuration.greenColor || "#109618";
        this.config.yellowColor = configuration.yellowColor || "#FF9900";
        this.config.redColor    = configuration.redColor || "#DC3912";
        
        this.config.transitionDuration = configuration.transitionDuration || 8000;
    }

    this.render = function()
    {
        this.body = d3.select("#" + this.placeholderName)
                            .append("svg:svg")
                            .attr("class", "gauge")
                            .attr("width", this.config.size)
                            .attr("height", this.config.size);
        
        this.body.append("svg:circle")
                    .attr("cx", this.config.cx)
                    .attr("cy", this.config.cy)
                    .attr("r", this.config.raduis)
                    .style("fill", "#000")
                    .style("stroke", "#000")
                    .style("stroke-width", "0.5px");
                    
        this.body.append("svg:circle")
                    .attr("cx", this.config.cx)
                    .attr("cy", this.config.cy)
                    .attr("r", 0.9 * this.config.raduis)
                    .style("fill", "#fff")
                    .style("stroke", "#000000")
                    .style("stroke-width", "2px");
                    
        for (var index in this.config.greenZones)
        {
            this.drawBand(this.config.greenZones[index].from, this.config.greenZones[index].to, self.config.greenColor);
        }

        for (var index in this.config.blueZones)
        {
            this.drawBand(this.config.blueZones[index].from, this.config.blueZones[index].to, self.config.blueColor);
        }
        
        for (var index in this.config.yellowZones)
        {
            this.drawBand(this.config.yellowZones[index].from, this.config.yellowZones[index].to, self.config.yellowColor);
        }
        
        for (var index in this.config.redZones)
        {
            this.drawBand(this.config.redZones[index].from, this.config.redZones[index].to, self.config.redColor);
        }
        
        if (undefined != this.config.label)
        {
            var fontSize = Math.round(this.config.size / 9);
            this.body.append("svg:text")
                        .attr("x", this.config.cx)
                        .attr("y", this.config.cy / 2 + fontSize / 2)
                        .attr("dy", fontSize / 2)
                        .attr("text-anchor", "middle")
                        .text(this.config.label)
                        .style("font-size", fontSize + "px")
                        .style("fill", "#000")
                        .style("stroke-width", "0px");
        }
        
        var fontSize = Math.round(this.config.size / 16);
        var majorDelta = this.config.range / (this.config.majorTicks - 1);
        for (var major = this.config.min; major <= this.config.max; major += majorDelta)
        {
            var minorDelta = majorDelta / this.config.minorTicks;
            for (var minor = major + minorDelta; minor < Math.min(major + majorDelta, this.config.max); minor += minorDelta)
            {
                var point1 = this.valueToPoint(minor, 0.75);
                var point2 = this.valueToPoint(minor, 0.85);
                
                this.body.append("svg:line")
                            .attr("x1", point1.x)
                            .attr("y1", point1.y)
                            .attr("x2", point2.x)
                            .attr("y2", point2.y)
                            .style("stroke", "#FFF")
                            .style("stroke-width", "1px");
            }
            
            var point1 = this.valueToPoint(major, 0.7);
            var point2 = this.valueToPoint(major, 0.85);    
            
            this.body.append("svg:line")
                        .attr("x1", point1.x)
                        .attr("y1", point1.y)
                        .attr("x2", point2.x)
                        .attr("y2", point2.y)
                        .style("stroke", "#333")
                        .style("stroke-width", "2px");
            
            if (major == this.config.min || major == this.config.max)
            {
                var point = this.valueToPoint(major, 0.63);
                
                this.body.append("svg:text")
                            .attr("x", point.x)
                            .attr("y", point.y)
                            .attr("dy", fontSize / 3)
                            .attr("text-anchor", major == this.config.min ? "start" : "end")
                            .text(major)
                            .style("font-size", fontSize + "px")
                            .style("fill", "#000")
                            .style("stroke-width", "0px");
            }
        }
        
        var pointerContainer = this.body.append("svg:g").attr("class", "pointerContainer");
        
        var midValue = (this.config.min + this.config.max) / 2;
        
        var pointerPath = this.buildPointerPath(midValue);
        
        var pointerLine = d3.svg.line()
                                    .x(function(d) { return d.x })
                                    .y(function(d) { return d.y })
                                    .interpolate("basis");
        
        pointerContainer.selectAll("path")
                            .data([pointerPath])
                            .enter()
                                .append("svg:path")
                                    .attr("d", pointerLine)
                                    .style("fill", "#000000")
                                    .style("stroke", "#000000")
                                    .style("fill-opacity", 0.7)
                    
        pointerContainer.append("svg:circle")
                            .attr("cx", this.config.cx)
                            .attr("cy", this.config.cy)
                            .attr("r", 0.12 * this.config.raduis)
                            .style("fill", "#000000")
                            .style("stroke", "#000")
                            .style("opacity", 1);
        
        var fontSize = Math.round(this.config.size / 10);
        pointerContainer.selectAll("text")
                            .data([midValue])
                            .enter()
                                .append("svg:text")
                                    .attr("x", this.config.cx)
                                    .attr("y", this.config.size - this.config.cy / 4 - fontSize)
                                    .attr("dy", fontSize / 2)
                                    .attr("text-anchor", "middle")
                                    .style("font-size", fontSize + "px")
                                    .style("fill", "#000")
                                    .style("stroke-width", "0px");
        
        this.redraw(this.config.min, 0);
    }
    
    this.buildPointerPath = function(value)
    {
        var delta = this.config.range / 13;
        
        var head = valueToPoint(value, 0.63);
        var head1 = valueToPoint(value - delta, 0.12);
        var head2 = valueToPoint(value + delta, 0.12);
        
        var tailValue = value - (this.config.range * (1/(270/360)) / 2);
        var tail = valueToPoint(tailValue, 0.20);
        var tail1 = valueToPoint(tailValue - delta, 0.12);
        var tail2 = valueToPoint(tailValue + delta, 0.12);
        
        return [head, head1, tail2, tail, tail1, head2, head];
        
        function valueToPoint(value, factor)
        {
            var point = self.valueToPoint(value, factor);
            point.x -= self.config.cx;
            point.y -= self.config.cy;
            return point;
        }
    }
    
    this.drawBand = function(start, end, color)
    {
        if (0 >= end - start) return;
        
        this.body.append("svg:path")
                    .style("fill", color)
                    .attr("d", d3.svg.arc()
                        .startAngle(this.valueToRadians(start))
                        .endAngle(this.valueToRadians(end))
                        .innerRadius(0.65 * this.config.raduis)
                        .outerRadius(0.85 * this.config.raduis))
                    .attr("transform", function() { return "translate(" + self.config.cx + ", " + self.config.cy + ") rotate(270)" });
    }
    
    this.redraw = function(value, transitionDuration)
    {
        var pointerContainer = this.body.select(".pointerContainer");
        
        pointerContainer.selectAll("text").text(Math.round(value));
        
        var pointer = pointerContainer.selectAll("path");
        pointer.transition()
                    .duration(undefined != transitionDuration ? transitionDuration : this.config.transitionDuration)
                    //.delay(0)
                    //.ease("linear")
                    //.attr("transform", function(d) 
                    .attrTween("transform", function()
                    {
                        var pointerValue = value;
                        if (value > self.config.max) pointerValue = self.config.max + 0.02*self.config.range;
                        else if (value < self.config.min) pointerValue = self.config.min - 0.02*self.config.range;
                        var targetRotation = (self.valueToDegrees(pointerValue) - 90);
                        var currentRotation = self._currentRotation || targetRotation;
                        self._currentRotation = targetRotation;
                        
                        return function(step) 
                        {
                            var rotation = currentRotation + (targetRotation-currentRotation)*step;
                            return "translate(" + self.config.cx + ", " + self.config.cy + ") rotate(" + rotation + ")"; 
                        }
                    });
    }
    
    this.valueToDegrees = function(value)
    {
        // thanks @closealert
        //return value / this.config.range * 270 - 45;
        return value / this.config.range * 270 - (this.config.min / this.config.range * 270 + 45);
    }
    
    this.valueToRadians = function(value)
    {
        return this.valueToDegrees(value) * Math.PI / 180;
    }
    
    this.valueToPoint = function(value, factor)
    {
        return {    x: this.config.cx - this.config.raduis * factor * Math.cos(this.valueToRadians(value)),
                    y: this.config.cy - this.config.raduis * factor * Math.sin(this.valueToRadians(value))      };
    }
    
    // initialization
    this.configure(configuration);  
}