package json.orgjson;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.Arrays;

public class JSONObjectTest {
    public static void main(String [] args){
        JSONObject j1 = new JSONObject("{name:'Nan Qin', 'gender':true}");
        System.out.println(j1.getString("name"));
        System.out.println("Gender: " + (j1.getBoolean("gender")? "Male":"Female"));

        DataBean d= new DataBean();
        JSONObject j2 = new JSONObject(d); // create from a java bean object
        System.out.println(j2);

        Data d2 = new Data();
        JSONObject j3 = new JSONObject(d2, new String[]{"success", "reasons", "value"});
        System.out.println(j3);
        System.out.println(j3.get("reasons").getClass());
        if(j3.get("reasons") instanceof String[]){
            System.out.println("true");
        }
    }
}

/**/
