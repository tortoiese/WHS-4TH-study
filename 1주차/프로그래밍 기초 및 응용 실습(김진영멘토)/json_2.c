#include <stdio.h>
#include "json_c.c"


int main(void)
{   
    const char *str = "{\"id\": \"0001\",\"type\": \"donut\",\"name\": \"Cake\",\"ppu\": 0.55,\"batters\": {\"batter\": [{\"id\": \"1001\", \"type\": \"Regular\"},{\"id\": \"1002\", \"type\": \"Chocolate\"},{\"id\": \"1003\", \"type\": \"Blueberry\"},{\"id\": \"1004\", \"type\": \"Devil's Food\"}]}, \"topping\": [{\"id\": \"5001\", \"type\": \"None\"},{\"id\": \"5002\", \"type\": \"Glazed\"},{\"id\": \"5005\", \"type\": \"Sugar\"},{\"id\": \"5007\", \"type\": \"Powdered Sugar\"},{\"id\": \"5006\", \"type\": \"Chocolate with Sprinkles\"},{\"id\": \"5003\", \"type\": \"Chocolate\"},{\"id\": \"5004\", \"type\": \"Maple\"}]}";
	//Creation and assignment of a json_value
	//Dynamic allocation used
	json_value json = json_create(str);

	// json -> "batters" -> "batter" 배열을 꺼낸다
	json_value batter = json_get(json, "batters", "batter");
	int batter_size = json_len(batter);

	// 배열을 하나씩 돌면서 각 항목의 id, type 출력
	for(int i=0; i<batter_size; i++)
	{
		json_value item = json_get(batter, i);
		char *id = json_get_string(item, "id");
		char *type = json_get_string(item, "type");
		printf("id: %s\n", id);
		printf("type: %s\n", type);
	}

	return 0;
}

