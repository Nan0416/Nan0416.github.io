package json.jackson;

import java.util.*;
public class ResponseBean {
    private boolean success;
    private String name;
    private String [] reasons;
    private Map<String, Object> values;

    @Override
    public String toString(){
        return "name: " + name + "; success: " + success;
    }
    public boolean isSuccess(){
        return this.success;
    }

    public void setName(String name){
        this.name = name;
    }
    public String getName() {
        return name;
    }
    public String[] getReasons(){
        return new String[0];
    }

    public Map<String, Object> getValues() {
        return values;
    }

    public void setSuccess(boolean success) {
        this.success = success;
    }

    public void setReasons(String[] reasons) {
        this.reasons = reasons;
    }

    public void setValues(Map<String, Object> values) {
        this.values = values;
    }
}
