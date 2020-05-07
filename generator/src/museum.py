from typing import List
from entry import Entry
from user import User, existing_users
from faker import Faker
from faker.providers import lorem
from collection import Collection
from artifact import Artifact
import requests


class Museum:
    urls = ["painting", "sculpture", "museum", "art", "vatican"]

    def __init__(self, user: User, entry=None, choice=0, lock=0, fake=Faker()):
        self.id = -1
        self.user = user
        self.collections = []
        if entry is None:
            fake.add_provider(lorem)
            title_list = [i.capitalize() for i in fake.words(3)]
            title = " ".join(title_list) + " Museum"
            introduction = fake.text()
            description = "**Description:** " + fake.paragraph(10)
            image = "https://loremflickr.com/600/400/"
            image += self.urls[choice] + "?lock=" + str(lock)
            self.entry = Entry(title, introduction, image, description)
        else:
            self.entry = entry

    def __str__(self):
        return self.entry.__str__() + "," + self.user.username + "," + str(self.id)

    def __dict__(self):
        return {
            "user": self.user.__dict__(),
            "museum": self.entry.__dict__()
        }

    def add_collection(self, choice=0, lock=0, fake=Faker()):
        collection = Collection(self, choice=choice, lock=lock, fake=fake)
        r = requests.post("http://127.0.0.1:5300/request/add-collection",
                          json=collection.__dict__())
        print(r.text)
        self.collections.append(collection)

    def get_collections_ids(self):
        r = requests.get("http://127.0.0.1:5300/request/museum/" + str(self.id))
        collections_list = r.json()["collectionList"]
        for collection in self.collections:
            for collection_json in collections_list:
                if collection.entry.name == collection_json["name"]:
                    collection.id = collection_json["id"]

    def add_artifact(self, collections, choice=0, lock=0, fake=Faker()):
        artifact = Artifact(self, collections, choice=choice, lock=lock, fake=Faker())
        r = requests.post("http://127.0.0.1:5300/request/add-artifact",
                          json=artifact.__dict__())
        print(r.text)


existing_museums = [
    Museum(existing_users[0], Entry("Museum by Pfaffmann", "The museum hand curated by Professor Pfaffmann",
                                    "https://images.unsplash.com/photo-1482245294234-b3f2f8d5f1a4?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=1730&q=80")),
    Museum(existing_users[1], Entry("Museum by Malo", "The museum hand curated by Malo",
                                    "https://images.unsplash.com/photo-1554907984-15263bfd63bd?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=1500&q=80")),
    Museum(existing_users[2], Entry("Museum by Lekso", "The museum hand curated by Lekso",
                                    "https://images.unsplash.com/photo-1518998053901-5348d3961a04?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=1334&q=80")),
    Museum(existing_users[3], Entry("Museum by Peter", "The museum hand curated by Peter",
                                    "https://images.unsplash.com/photo-1555080313-45e32dbb9927?ixlib=rb-1.2.1&auto=format&fit=crop&w=1500&q=80")),
    Museum(existing_users[4], Entry("Museum by Sena", "The museum hand curated by Sena",
                                    "https://images.unsplash.com/photo-1560579210-69248380602a?ixlib=rb-1.2.1&auto=format&fit=crop&w=1506&q=80"))
]

filename = "museums.tsv"


def write_and_add_museums(museums: List[Museum]) -> None:
    f = open(filename, "w+")
    f.write('name\tintroduction\timage\tdescription\tcuratorUsername\n')
    for museum in museums:
        r = requests.post("http://127.0.0.1:5300/request/add-museum",
                          json=museum.__dict__())
        print(r.text)
        f.write(museum.__str__())
    print("Mock user data written to " + filename)


def get_museum_ids(museums: List[Museum]):
    r = requests.get("http://127.0.0.1:5300/request/museum-list")
    museums_list = r.json()
    for museum in museums:
        for museum_json in museums_list:
            if museum.entry.name == museum_json["name"]:
                museum.id = museum_json["id"]
    return museums
