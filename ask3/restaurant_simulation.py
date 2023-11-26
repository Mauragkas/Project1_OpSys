#!/usr/bin/env python3

# 1093513 Christodoulopoulos Efstathios Panagiotis
# 1093514 Christodoulo NIkolaos
# _______ Basilopoulos Basilios

import threading

# Assuming X and Y are calculated based on student registration numbers
X = 3  # Example value
Y = 5  # Example value

# Shared Resources and Locks
servings_in_pot = X + 5
ingredients_in_kitchen = {'beans': 0, 'carrots': 0}
pot_lock = threading.Lock()
kitchen_lock = threading.Lock()

# Flags
is_cooking = False

def take_food(tenant_id):
    global servings_in_pot
    while True:
        with pot_lock:
            if servings_in_pot > 0:
                servings_in_pot -= 1
                print(f"Tenant {tenant_id} took a serving. Servings left: {servings_in_pot}")
                if servings_in_pot == 0:
                    inform_cook()

def cook_food():
    global servings_in_pot, is_cooking
    while True:
        if not is_cooking and servings_in_pot == 0:
            is_cooking = True
            with kitchen_lock:
                if ingredients_in_kitchen['beans'] >= Y+3 and ingredients_in_kitchen['carrots'] >= Y:
                    print("Cooking food...")
                    # Simulate cooking time
                    threading.Event().wait(2)
                    servings_in_pot = X + 5
                    ingredients_in_kitchen['beans'] -= Y+3
                    ingredients_in_kitchen['carrots'] -= Y
                    print("Food is ready!")
            is_cooking = False

def helper(assistant_id, material):
    while True:
        with kitchen_lock:
            if ingredients_in_kitchen[material] < Y + (3 if material == 'beans' else 0):
                ingredients_in_kitchen[material] += Y
                print(f"Assistant {assistant_id} brought {Y}kg of {material}. Total {material}: {ingredients_in_kitchen[material]}kg")

def inform_cook():
    print("Pot is empty, informing the cook.")

# Create and start threads
tenant_threads = [threading.Thread(target=take_food, args=(i,)) for i in range(10)]
cook_thread = threading.Thread(target=cook_food)
assistant_threads = [threading.Thread(target=helper, args=(i, 'beans' if i % 2 == 0 else 'carrots')) for i in range(2)]

for t in tenant_threads:
    t.start()
cook_thread.start()
for a in assistant_threads:
    a.start()
