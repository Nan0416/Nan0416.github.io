package json.jackson;
import com.fasterxml.jackson.databind.ObjectMapper;
import java.io.IOException;
import java.util.*;
public class JacksonTest {

    public static void main(String [] args){
        String x = "{\"name\": \" Nan Qin \", \"success\": true, \"values\": {\"age\":21}}";
        ObjectMapper mapper = new ObjectMapper();
        try{
            ResponseBean rb = mapper.readValue(x, ResponseBean.class);
            System.out.println(rb.getValues().get("age"));
            if(rb.getValues() instanceof HashMap){
                System.out.println("hash");
            }
        }catch (IOException e){
            System.out.println(e.getMessage());
        }
    }
}
