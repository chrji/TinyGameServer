create table if not exists GameUser
(
    UID      bigint primary key,
    USERNAME text not null,
    PASSWORD text not null
);

insert into GameUser(UID, USERNAME, PASSWORD)
values (100, 'admin', '123456');

insert into GameUser(UID, USERNAME, PASSWORD)
values (101, 'user', '123456');


create table if not exists UserAttribute
(
    UID   bigint primary key,
    ATK   int not null,
    DEF   int not null,
    APD   int not null,
    HIT   int not null,
    Level int not null
);

insert into UserAttribute(UID, ATK, DEF, APD, HIT, Level)
values (100, 1, 1, 1, 1, 1);

insert into UserAttribute(UID, ATK, DEF, APD, HIT, Level)
values (101, 2, 2, 2, 2, 2);


-- TODO