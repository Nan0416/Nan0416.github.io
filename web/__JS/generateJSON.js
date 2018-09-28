var NAME_KEY_CONSTANT = {
  tierName: "tier_name",
  tierSize: "tier_size",
  tierType: "tier_type",
  tierLocation: "tier_location",
  tierExpectedLatency: "tier_expected_latency",
  tierEvents: "events",
  tierEventType: "event_type",
  tierEventParameters: "event_parameters",
  tierEventResponses: "responses"
};
var TIERA_EVENT_CONSTANT = {};
var WIERA_EVENT_CONSTANT = {};

function Wiera(){
  this.wieraPolicy = {};
  this.idKey = "id";
  this.descKey = "desc";
  this.localInstancesKey = "local_instances";
  this.toJSON = function(){
    return JSON.stringify(this.wieraPolicy, null, 4);
  }
  this.configuration = function(id, desc){
    this.wieraPolicy[this.idKey] = id;
    this.wieraPolicy[this.descKey] = desc;
    this.wieraPolicy[this.localInstancesKey] = new Object();
  }
  this.addARegionalTiera = function(region){


    this.wieraPolicy[this.localInstancesKey][region] = new Object();
    return new Tiera(this, region);

  }
}


function Tiera(wiera, region){
  this.wiera = wiera;
  this.region = region;
  this.tierOrder = new Array();
  this.counter = 0;
  this.tieraName = null;
  this.wieraTieraPolicyDelegate = null;
  this.setName = function(name){
    this.tieraName = name;
    this.wiera.wieraPolicy[this.wiera.localInstancesKey][this.region][this.tieraName] = new Array();
    this.wiera.wieraPolicy[this.wiera.localInstancesKey][this.region][NAME_KEY_CONSTANT.tierEvents] =  new Array();
    this.wieraTieraPolicyDelegate = this.wiera.wieraPolicy[this.wiera.localInstancesKey][this.region];
  }

   this.addATier = function(args){
    var assemble = new Object();
    var i = 0;
    for(; i < args.length; i++){
      assemble[args[i][0]] = args[i][1];
    }
    this.wieraTieraPolicyDelegate[this.tieraName][this.counter] = assemble;
    this.counter += 1;
    
  }

  this.addAEvent = function(type, parameters, responses){
    var obj = new Object();
    obj[NAME_KEY_CONSTANT.tierEventType] = type;
    obj[NAME_KEY_CONSTANT.tierEventParameters] = parameters;
    obj[NAME_KEY_CONSTANT.tierEventResponses] = responses;
    var len = this.wieraTieraPolicyDelegate[NAME_KEY_CONSTANT.tierEvents].length;
    this.wieraTieraPolicyDelegate[NAME_KEY_CONSTANT.tierEvents][len] = obj;
  }
  //this.addAEvent = function
}
