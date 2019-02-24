package json.orgjson;

import java.util.*;
public class Data{
    public boolean success;
    public String [] reasons;
    public Object value;

    public Data(){
        success = false;
        reasons = new String [0];
        value = null;
    }
    public void setSuccess(boolean success) {
        this.success = success;
    }
    public void addReason(String reason){
        reasons = Arrays.copyOf(reasons, reasons.length + 1);
        reasons[reasons.length - 1] = reason;
    }
    public void setValue(Object x){
        this.value = x;
    }
}