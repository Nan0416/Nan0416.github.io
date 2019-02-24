package json.orgjson;

import org.json.JSONPropertyName;
public class DataBean {
    private boolean success;
    private String [] reasons;
    private Object value;

    public DataBean(){
        this.success = false;
        this.reasons = new String[0];
        this.value = null;
    }
    public void setSuccess(boolean success){
        this.success = success;
    }
    public boolean isSuccess(){
        return this.success;
    }
    public void setReasons(String [] reasons){
        this.reasons = reasons;
    }
    @JSONPropertyName("FullName")
    public String [] getReasons(){
        return this.reasons;
    }
    public void setValue(Object obj){
        this.value = obj;
    }
    public Object getValue(){
        return this.value;
    }
}
