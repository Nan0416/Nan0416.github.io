package json.jackson;
import com.fasterxml.jackson.databind.ObjectMapper;
public class JavaBean2String {
    public static void main(String [] args){
        ResponseBean rb = new ResponseBean();
        rb.setName("Nan Qin");
        ObjectMapper mapper = new ObjectMapper();

        try {
            String json = mapper.writeValueAsString(rb);
            System.out.println(json);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
