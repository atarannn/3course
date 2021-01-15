import redis
import csv
import os

r = redis.StrictRedis()
with open(os.path.expanduser("~/Desktop/redis.csv"),"w") as file:
    csv_w = csv.writer(file)
    for key in r.scan_iter():
        val = None
        vals = None
        r_type = r.type(key).decode("utf-8")
        if r_type == "string":
            val = r.get(key)
        if r_type == "hash":
            vals = r.hgetall(key)
        if r_type == "zset":
            vals = r.zrange(key, 0, -1)
        if r_type == "list":
            vals = r.lrange(key, 0, -1)
        if r_type == "set":
            vals = r.smembers(key)
        print((r_type, val, vals))
        if val:
            csv_w.writerow((key, val))
        elif vals:
            csv_w.writerow((key, vals))