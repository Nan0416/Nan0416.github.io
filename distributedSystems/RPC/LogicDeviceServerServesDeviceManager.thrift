service LogicDeviceServerServesDeviceManagerIface {
    string startInstances(1:string deviceInfo);
    string stopInstances(1:string deviceInfo);
}