#!/usr/bin/python

import db

cn = db.connect()
query = db.query(cn)

try:

    if not query.exec("""

drop table if exists video;

create table video
(
    hash char(32) primary key,
    name text unique,
    video_link text,
    metadata_link text
);

drop table if exists map;

create table map
(
    hash char(32) primary key,
    name text unique,
    map_link text,
    height_map_link text
);
    """):
        
        raise Exception

except:
    
    error = db.error(cn)
    print("[ Error ] %u %s" % (error.number(), error.text()))

