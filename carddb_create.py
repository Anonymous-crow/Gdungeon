#!/usr/bin/env python3
import sqlite3
import os
import click
import csv

class TransferDB():
    def __init__(self, 
                 input_file : str | None = None,
                 output_file : str | None = None,
                 ) -> None:
        if output_file == None:
            self.output = os.path.join(
                os.path.dirname(os.path.abspath(__file__)), "assets", "cards.db")
        else:
            self.output = output_file
        self.con = sqlite3.connect(self.output)
        self.data = self.con.cursor()
        self.create_table()
        if input_file:
            self.import_cards(input_file=input_file)

    def create_table(self) -> None:
        self.data.execute('''
CREATE TABLE IF NOT EXISTS "playerCards" (
	"name"	TEXT,
	"cardID"	TEXT NOT NULL UNIQUE,
	"tagline"	TEXT,
	"description"	TEXT,
	"position"	TEXT,
	"barrage"	BOOL NOT NULL CHECK("barrage" IN (0, 1)),
	"team"	BOOL NOT NULL CHECK("team" IN (0, 1)),
	"energyCost"	INTEGER NOT NULL,
	"token"	BOOL NOT NULL CHECK("token" IN (0, 1)),
	"tokenCost"	INTEGER NOT NULL,
	"tokenType"	TEXT,
	"damage"	INTEGER NOT NULL,
	"copies"	INTEGER NOT NULL,
	"repeat"	INTEGER NOT NULL,
	"exhaust"	BOOL NOT NULL CHECK(exhaust in (0,1)),
	PRIMARY KEY("cardID")
)
''')
        self.data.execute("CREATE UNIQUE INDEX IF NOT EXISTS idsort ON \"playerCards\" (\"cardID\"	ASC)")
        self.data.execute('''
CREATE TABLE IF NOT EXISTS "cardEffects" (
	"cardID"	TEXT NOT NULL UNIQUE,
	"shield"	INTEGER,
	"strength"	INTEGER,
	"fortitude"	INTEGER,
	"glint"	INTEGER,
    "mark"	INTEGER,
	"immunity"	INTEGER,
    "restore"	INTEGER,
    "clear"	INTEGER,
	"taunt"	INTEGER,
	"weak"	INTEGER,
	"vulnerable"	INTEGER,
	"stun"	INTEGER,
	PRIMARY KEY("cardID"),
	FOREIGN KEY("cardID") REFERENCES "playerCards"("cardID")
)
''')

    def read_csv(self, input_file : str) -> None:
        with open(input_file, "r") as f:
            return [row for row in csv.DictReader(f.readlines())]
        
    def import_cards(self, input_file : str) -> None:
        csvdata = self.read_csv(input_file)
        for i in csvdata:
            for j in ["barrage", "team", "token", "exhaust"]:
                i[j] = i[j] == "TRUE"
            self.data.execute("""INSERT OR REPLACE INTO "playerCards" values (
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
                                  "EXHAUST": i["exhaust"]
                              })
        self.con.commit()
    
    def import_effects(self, input_file : str) -> None:
        csvdata = self.read_csv(input_file)
        for i in csvdata:
            for j in []:
                i[j] = i[j] == "TRUE"
            self.data.execute("""INSERT OR REPLACE INTO "cardEffects" values (
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
                                  "STUN": i["stun"]
                              })
        self.con.commit()


    def __del__(self):
        self.con.close()

# if __name__ == "__main__":
#     obj = TransferDB("Cards - Sheet1.csv")
        
@click.group(invoke_without_command=True)
@click.pass_context
@click.argument("input", default="Cards - data.csv")
@click.option('-o', '--output', default=None)
@click.option('-e', '--effect-table', is_flag=True)
def cli(ctx,
        input : str | None,
        output : str | None,
        effect_table : bool) -> None:
    ctx.obj = TransferDB(output_file=output)
    ctx.show_default = True
    if (effect_table):
        ctx.obj.import_effects(input_file=input)
        click.echo(F"{input} copied to {ctx.obj.output}")
    else:
        ctx.obj.import_cards(input_file=input)
        click.echo(F"{input} copied to {ctx.obj.output}")

# @cli.command()
# @click.argument("output", default="card.db")
# @click.pass_obj
# def mktable(obj, output : str):
#     obj.create_table(output)

if __name__ == "__main__":
    cli()