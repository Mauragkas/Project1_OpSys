#!/usr/bin/env python3

# 1093513 Christodoulopoulos Efstathios Panagiotis
# 1093514 Christodoulo NIkolaos
# 1097445 Basilopoulos Basilios

import threading

AM = "1093513"

X = int(AM[6])  
Y = int(AM[4])

# Shared Resources and Locks
servings_in_pot = X + 5
ingredients_in_kitchen = {'beans': 0, 'carrots': 0}
pot_lock = threading.Lock() # Lock for the pot (servings_in_pot)
kitchen_lock = threading.Lock() # Lock for the kitchen (ingredients_in_kitchen)

# Flags and Events
is_cooking = False
tenant_has_eaten = [False for _ in range(10)]  # Flag for each tenant
pot_empty_event = threading.Event()  # Event to signal that the pot is empty
pot_empty_event.set()  # Initially set because pot is full
food_ready_event = threading.Event()  # Event to signal that the food is ready

def take_food(tenant_id):
    global servings_in_pot, tenant_has_eaten
    while True:
        food_ready_event.wait()  # Wait until food is ready
        with pot_lock:
            if servings_in_pot > 0 and not tenant_has_eaten[tenant_id]:
                servings_in_pot -= 1
                tenant_has_eaten[tenant_id] = True
                print(f"Tenant {tenant_id} took a serving. Servings left: {servings_in_pot}")
                if servings_in_pot == 0:
                    inform_cook()
                    food_ready_event.clear()  # Reset the food ready event

def cook_food():
    global servings_in_pot, is_cooking, tenant_has_eaten
    while True:
        pot_empty_event.wait()  # Wait for the pot to be empty
        is_cooking = True
        with kitchen_lock:
            if ingredients_in_kitchen['beans'] >= Y+3 and ingredients_in_kitchen['carrots'] >= Y:
                print("Cooking food...")
                # Simulate cooking time
                threading.Event().wait(2)
                servings_in_pot = X + 5
                tenant_has_eaten = [False for _ in range(10)]  # Reset the flags
                ingredients_in_kitchen['beans'] -= Y+3
                ingredients_in_kitchen['carrots'] -= Y
                print("Food is ready!")
                food_ready_event.set()  # Signal that food is ready
                pot_empty_event.clear()  # Reset the pot empty event
        is_cooking = False

def helper(assistant_id, material):
    while True:
        pot_empty_event.wait()  # Wait for the pot to be empty
        with kitchen_lock:
            if ingredients_in_kitchen[material] < Y + (3 if material == 'beans' else 0):
                ingredients_in_kitchen[material] += Y
                print(f"Assistant {assistant_id} brought {Y}kg of {material}. Total {material}: {ingredients_in_kitchen[material]}kg")

def inform_cook():
    print("Pot is empty, informing the cook.")
    pot_empty_event.set()  # Signal that the pot is empty

def main():
    # Create and start threads
    tenant_threads = [threading.Thread(target=take_food, args=(i,)) for i in range(10)]
    cook_thread = threading.Thread(target=cook_food)
    assistant_threads = [threading.Thread(target=helper, args=(i, 'beans' if i % 2 == 0 else 'carrots')) for i in range(2)]

    for t in tenant_threads:
        t.start()
    cook_thread.start()
    for a in assistant_threads:
        a.start()

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("Exiting...")
        exit(0)