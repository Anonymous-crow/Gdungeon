#!/usr/bin/env python3
import csv
import os
import sqlite3

import click


class TransferDB:
    def __init__(
        self,
        input_file: str | None = None,
        output_file: str | None = None,
    ) -> None:
        if output_file == None:
            self.output = os.path.join(
                os.path.dirname(os.path.abspath(__file__)), "assets", "cards.db"
            )
        else:
            self.output = output_file
        self.con = sqlite3.connect(self.output)
        self.data = self.con.cursor()
        self.create_table()
        if input_file:
            self.import_cards(input_file=input_file)

    def create_table(self) -> None:
        self.data.execute(
            """
CREATE TABLE IF NOT EXISTS "playerCards" (
        "name"	        TEXT,
        "cardID"	TEXT NOT NULL UNIQUE,
        "tagline"	TEXT,
        "description"	TEXT,
        "position"	TEXT,
        "barrage"	BOOL NOT NULL CHECK("barrage" IN (0, 1)),
        "team"	        BOOL NOT NULL CHECK("team" IN (0, 1)),
        "energyCost"	INTEGER NOT NULL,
        "token"	        BOOL NOT NULL CHECK("token" IN (0, 1)),
        "tokenCost"	INTEGER NOT NULL,
        "tokenType"	TEXT,
        "damage"	INTEGER NOT NULL,
        "copies"	INTEGER NOT NULL,
        "repeat"	INTEGER NOT NULL,
        "exhaust"	BOOL NOT NULL CHECK(exhaust in (0,1)),
        PRIMARY KEY("cardID")
)
"""
        )
        self.data.execute(
            'CREATE UNIQUE INDEX IF NOT EXISTS \
idsort ON "playerCards" ("cardID"	ASC)'
        )
        self.data.execute(
            """
CREATE TABLE IF NOT EXISTS "cardEffects" (
        "cardID"	TEXT NOT NULL UNIQUE,
        "shield"	INTEGER,
        "strength"	INTEGER,
        "fortitude"	INTEGER,
        "glint"	        INTEGER,
        "mark"	        INTEGER,
        "immunity"	INTEGER,
        "restore"	INTEGER,
        "clear"	        INTEGER,
        "taunt"	        INTEGER,
        "weak"	        INTEGER,
        "vulnerable"	INTEGER,
        "stun"	        INTEGER,
        PRIMARY KEY("cardID"),
        FOREIGN KEY("cardID") REFERENCES "playerCards"("cardID")
)
"""
        )
        self.data.execute(
            """
CREATE TABLE IF NOT EXISTS "enemies" (
        "name"	        TEXT,
        "enemyID"	TEXT NOT NULL UNIQUE,
        "HP"	        INTEGER NOT NULL,
        "size"	        INTEGER NOT NULL,
        "range"	        INTEGER NOT NULL,
        "A"             TEXT,
        "B"             TEXT,
        PRIMARY KEY("enemyID")
)
"""
        )
        self.data.execute(
            """
CREATE TABLE IF NOT EXISTS "enemyCards" (
        "name"	        TEXT,
        "cardID"	TEXT NOT NULL UNIQUE,
        "damage"	INTEGER NOT NULL,
        "token"         BOOL NOT NULL CHECK("token" IN (0,1)),
        "tokenType"     TEXT,
        "tokenGain"     TEXT,
        PRIMARY KEY("cardID")
)
"""
        )
        self.data.execute(
            """
CREATE TABLE IF NOT EXISTS "enemyEffects" (
        "cardID"	TEXT NOT NULL UNIQUE,
        "shield"	INTEGER,
        "strength"	INTEGER,
        "fortitude"	INTEGER,
        "glint" 	INTEGER,
        "mark"	        INTEGER,
        "immunity"	INTEGER,
        "restore"	INTEGER,
        "clear"	        INTEGER,
        "taunt"	        INTEGER,
        "weak"	        INTEGER,
        "vulnerable"	INTEGER,
        "stun"	        INTEGER,
        PRIMARY KEY("cardID"),
        FOREIGN KEY("cardID") REFERENCES "enemyCard"("cardID")
)
"""
        )

    def read_csv(self, input_file: str) -> list[dict[str, str | bool]]:
        with open(input_file, "r") as f:
            return [row for row in csv.DictReader(f.readlines())]

    def import_cards(self, input_file: str) -> None:
        csvdata = self.read_csv(input_file)
        for i in csvdata:
            for j in ["barrage", "team", "token", "exhaust"]:
                i[j] = i[j] == "TRUE"
            self.data.execute(
                """INSERT OR REPLACE INTO "playerCards" values (
                :NAME,
                :CARDID,
                :TAGLINE,
                :DESCRIPTION,
                :POSITION,
                :BARRAGE,
                :TEAM,
                :ENERGYCOST,
                :TOKEN,
                :TOKENCOST,
                :TOKENTYPE,
                :DAMAGE,
                :COPIES,
                :REPEAT,
                :EXHAUST
                )""",
                {
                    "NAME": i["name"],
                    "CARDID": i["cardID"],
                    "TAGLINE": i["tagline"],
                    "DESCRIPTION": i["description"],
                    "POSITION": i["position"],
                    "BARRAGE": i["barrage"],
                    "TEAM": i["team"],
                    "ENERGYCOST": i["energyCost"],
                    "TOKEN": i["token"],
                    "TOKENCOST": i["tokenCost"],
                    "TOKENTYPE": i["tokenType"],
                    "DAMAGE": i["damage"],
                    "COPIES": i["copies"],
                    "REPEAT": i["repeat"],
                    "EXHAUST": i["exhaust"],
                },
            )
        self.con.commit()

    def import_effects(self, input_file: str) -> None:
        csvdata = self.read_csv(input_file)
        for i in csvdata:
            self.data.execute(
                """INSERT OR REPLACE INTO "cardEffects" values (
                :CARDID,
                :SHIELD,
                :STRENGTH,
                :FORTITUDE,
                :GLINT,
                :MARK,
                :IMMUNITY,
                :RESTORE,
                :CLEAR,
                :TAUNT,
                :WEAK,
                :VULNERABLE,
                :STUN
                )""",
                {
                    "CARDID": i["cardID"],
                    "SHIELD": i["shield"],
                    "STRENGTH": i["strength"],
                    "FORTITUDE": i["fortitude"],
                    "GLINT": i["glint"],
                    "MARK": i["mark"],
                    "IMMUNITY": i["immunity"],
                    "RESTORE": i["restore"],
                    "CLEAR": i["clear"],
                    "TAUNT": i["taunt"],
                    "WEAK": i["weak"],
                    "VULNERABLE": i["vulnerable"],
                    "STUN": i["stun"],
                },
            )
        self.con.commit()

    def import_enemies(self, input_file: str) -> None:
        csvdata = self.read_csv(input_file)
        for i in csvdata:
            self.data.execute(
                """INSERT OR REPLACE INTO "enemies" VALUES (
                :NAME,
                :ENEMYID,
                :HP,
                :SIZE,
                :RANGE,
                :A,
                :B
                )""",
                {
                    "NAME": i["name"],
                    "ENEMYID": i["enemyID"],
                    "HP": i["HP"],
                    "SIZE": i["size"],
                    "RANGE": i["range"],
                    "A": i["A"],
                    "B": i["B"],
                },
            )
        self.con.commit()

    def import_enemy_cards(self, input_file: str) -> None:
        csvdata = self.read_csv(input_file)
        for i in csvdata:
            i["token"] = i["token"] == "TRUE"
            self.data.execute(
                """INSERT OR REPLACE INTO "enemyCards" VALUES (
                :NAME,
                :CARDID,
                :DAMAGE,
                :TOKEN,
                :TOKENTYPE,
                :TOKENGAIN
                )""",
                {
                    "NAME": i["name"],
                    "CARDID": i["cardID"],
                    "DAMAGE": i["damage"],
                    "TOKEN": i["token"],
                    "TOKENTYPE": i["tokenType"],
                    "TOKENGAIN": i["tokenGain"],
                },
            )
        self.con.commit()

    def import_enemy_effects(self, input_file: str) -> None:
        csvdata = self.read_csv(input_file)
        for i in csvdata:
            self.data.execute(
                """INSERT OR REPLACE INTO "enemyEffects" values (
                :CARDID,
                :SHIELD,
                :STRENGTH,
                :FORTITUDE,
                :GLINT,
                :MARK,
                :IMMUNITY,
                :RESTORE,
                :CLEAR,
                :TAUNT,
                :WEAK,
                :VULNERABLE,
                :STUN
                )""",
                {
                    "CARDID": i["cardID"],
                    "SHIELD": i["shield"],
                    "STRENGTH": i["strength"],
                    "FORTITUDE": i["fortitude"],
                    "GLINT": i["glint"],
                    "MARK": i["mark"],
                    "IMMUNITY": i["immunity"],
                    "RESTORE": i["restore"],
                    "CLEAR": i["clear"],
                    "TAUNT": i["taunt"],
                    "WEAK": i["weak"],
                    "VULNERABLE": i["vulnerable"],
                    "STUN": i["stun"],
                },
            )
        self.con.commit()

    def __del__(self):
        self.con.close()


# if __name__ == "__main__":
#     obj = TransferDB("Cards - Sheet1.csv")


@click.group(invoke_without_command=True)
@click.pass_context
@click.argument("input", default="Cards - data.csv", type=click.Path())
@click.option(
    "-o",
    "--output",
    default=None,
    help=" ".join(
        "Specify a location to save the database to.\
              The default is './assets/cards.db'.".split()
    ),
)
@click.option(
    "-e",
    "--effect-table",
    is_flag=True,
    help=" ".join(
        "a flag that creates a Card Effect \
              table in the database instead of the usual Cards \
              table.  This requires a different CSV than the \
              Cards table.".split()
    ),
)
@click.option("--enemies", is_flag=True, help="Populates the enemies table.")
@click.option("--enemy-cards", is_flag=True, help="Populates the enemyCards table.")
@click.option("--enemy-effects", is_flag=True, help="Populates the enemyEffects table.")
def cli(
    ctx,
    input: str,
    output: str | None,
    effect_table: bool,
    enemies: bool,
    enemy_cards: bool,
    enemy_effects: bool,
) -> None:
    """Turn a CSV full of Gangster Dungeon Cards into a \
SQLite Database of Gangster Dungeon Cards.

INPUT is the path to the CSV.
    """
    ctx.obj = TransferDB(output_file=output)
    ctx.show_default = True
    if effect_table:
        ctx.obj.import_effects(input_file=input)
        click.echo(f"{input} copied to {ctx.obj.output}")
    elif enemies:
        ctx.obj.import_enemies(input_file=input)
        click.echo(f"{input} copied to {ctx.obj.output}")
    elif enemy_cards:
        ctx.obj.import_enemy_cards(input_file=input)
        click.echo(f"{input} copied to {ctx.obj.output}")
    elif enemy_effects:
        ctx.obj.import_enemy_effects(input_file=input)
        click.echo(f"{input} copied to {ctx.obj.output}")

    else:
        ctx.obj.import_cards(input_file=input)
        click.echo(f"{input} copied to {ctx.obj.output}")


# @cli.command()
# @click.argument("output", default="card.db")
# @click.pass_obj
# def mktable(obj, output : str):
#     obj.create_table(output)

if __name__ == "__main__":
    cli()
