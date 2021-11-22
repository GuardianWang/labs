import maya.cmds as cmds
import random
import math
from itertools import combinations

## HELPER METHODS ##

def moveEmitters(emitters):
    for instanceResult in emitters:
        x = random.uniform(-10, 10)
        y = random.uniform(0, 20)
        z = random.uniform(-10, 10)

        cmds.move(x, y, z, instanceResult)


def createAndGetEmitters():
    emittersToReturn = []
    # TODO 1: Create 50 particle emitters using a for loop. Each particle emitter should have a unique name.
    # HINT 1: see the previous section on how to create and name a particle emitter via scripting (step 5)
    # HINT 2: to set the rate use this command e.g: cmds.setAttr("%s.rate" % emitter1[1], 300)
    for i in range(50):
        emitter_name = "emitter{:02d}".format(i)
        particle_emitter = cmds.emitter(pos=(0,5,0), type="omni", rate=300, n=emitter_name)
        emittersToReturn.append(particle_emitter)
    return emittersToReturn


def getDistance(emitter1, emitter2):
    diffX = cmds.getAttr("%s.translateX" % emitter1[0]) - cmds.getAttr("%s.translateX" % emitter2[0])
    diffY = cmds.getAttr("%s.translateY" % emitter1[0]) - cmds.getAttr("%s.translateY" % emitter2[0])
    diffZ = cmds.getAttr("%s.translateZ" % emitter1[0]) - cmds.getAttr("%s.translateZ" % emitter2[0])

    return math.sqrt((diffX * diffX) + (diffY * diffY) + (diffZ * diffZ))


def print_coord(cur_emitter):
    print([cmds.getAttr('{}.translate{}'.format(cur_emitter[0], x)) for x in "XYZ"])


def emitOnCloseContact(emitters):
    particled = set()
    for emitter1, emitter2 in combinations(emitters, 2):
        # TODO 2: if the distance between 2 emitters is less than 1, tell the emitters to emit particles, if not, tell them to not emit particles
        # Hint 1: Use the getDistance method provided to compute distance
        # Hint 2: Make sure you aren’t comparing the distance between the same emitter
        dist = getDistance(emitter1, emitter2)
        if dist < 1:
            for cur_emitter in [emitter1, emitter2]:
                if cur_emitter[0] in particled:
                    continue
                particle_name = cur_emitter[0].replace("emitter", "particle")
                cmds.particle( n=particle_name )
                cmds.connectDynamic( particle_name, em=cur_emitter[0])
                particled.add(cur_emitter[0])


def check_dist(emitters):
    for emitter1, emitter2 in combinations(emitters, 2):
        dist = getDistance(emitter1, emitter2)
        if dist < 1:
            print(emitter1[0], emitter2[0])

## SCENE SETUP AND FRAME EXECUTION ##

emitters = createAndGetEmitters()

for frame in range(1, 5):
    cmds.currentTime(frame)
    moveEmitters(emitters)
    emitOnCloseContact(emitters)
