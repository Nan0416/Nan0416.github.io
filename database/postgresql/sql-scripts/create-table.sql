create or replace function query_actor(skip_ integer, limit_ integer)
    returns table(firstname character varying(45),
                 lastname character varying(45))
as $$
begin
    return query select first_name, last_name from public.actor limit limit_ offset skip_;
end; 
$$
LANGUAGE 'plpgsql';