import requests
import json
#ACCESS_TOKEN = "CAACEdEose0cBAEqchKMZBolZAN9ADiWpoyjHQu3vQHhrcimPMnKONA7LYZCO6NJsnFqer1CWGqIzS6DXZBnnbVUa6oNLCVHdpdnoGANHJ7bO0a74O21ptngmPh1S9TTLrsADmb5tzzr7txWqagEZCpygseCwWzZCHiMA3l7DRP50ZCV6eHiJZA6l28oiXhPH7cNTduWIOon25mQuNo1UA9LT4IguOOlcZAtEZD"
ACCESS_TOKEN = "CAAKbXnsGZA3gBAHTlJE5P1SoUapiVs4zLtgkfenbpHRUxOQNpDeUWwltZBFNwSnK3qxBZBZBdbQEtSYMrwQYAip9fppI5P0IdQp5QTxkZAEeZAezwOLkZBW7jooU2ZAYpxsYhKmsTzYg0Uyq5dInQiKVYnpRirP4kIUjHNK4PL9RIJ1QlZBdD1gOleSPqUjQpNI5qjd0vI09EZCUDZAqWMVOJXWNgscb7FkecoZD"
query_statement = {"q1": "SELECT message FROM stream WHERE source_id = me()"}
query_json = json.dumps(query_statement)

#payload = {"q": query_json, "access_token":ACCESS_TOKEN}
#payload = {"q": query_json, "access_token":ACCESS_TOKEN}
payload = {"q": query_json, "access_token":ACCESS_TOKEN}

try:
    query = requests.get("https://graph.facebook.com/wiced123/posts?access_token="+ACCESS_TOKEN)
except:
    print "Connection Error !!"

if not ("200" in str(query)):
    print query
    print "Connection Issues!!"

result_set = query.json()
# print result_set
# print json.dumps(result_set, indent = 4)
#print result_set["data"]
#print result_set["data"]
for post in result_set["data"]:
	#print post
	if 'message' in post:
		print post['message']
# print result_set
#print result_set["data"][0]["fql_result_set"][0]["message"]
#print json.dumps(result_set)