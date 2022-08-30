import glob
import os
import sys
import argparse
try:
    sys.path.append(glob.glob('../carla/dist/carla-*%d.%d-%s.egg' % (
        sys.version_info.major,
        sys.version_info.minor,
        'win-amd64' if os.name == 'nt' else 'linux-x86_64'))[0])
except IndexError:
    pass

import carla

import random
import time


def main():
    try:
        argparser = argparse.ArgumentParser(
            description=__doc__)
        argparser.add_argument(
            '--host',
            metavar='H',
            default='127.0.0.1',
            help='IP of the host server (default: 127.0.0.1)')
        argparser.add_argument(
            '-p', '--port',
            metavar='P',
            default=2000,
            type=int,
            help='TCP port to listen to (default: 2000)')
    
        args = argparser.parse_args()


        client = carla.Client(args.host, args.port)
        client.set_timeout(2.0)
        world = client.get_world()

        settings = world.get_settings()
        settings.fixed_delta_seconds = 0.1  # Set a variable time-step
        world.apply_settings(settings)
 

        bp = random.choice(world.get_blueprint_library().filter('vehicle'))
        if bp.has_attribute('color'):
            color = random.choice(bp.get_attribute('color').recommended_values)
            bp.set_attribute('color', color)      
        transform = random.choice(world.get_map().get_spawn_points())        
        vehicle = world.spawn_actor(bp, transform)

        print('created %s' % vehicle.type_id)
       
        vehicle.set_autopilot(True)
   

        driver_bp = world.get_blueprint_library().find('sensor.other.driver')

        # Step 1: you can set attributes of the driver unit here
        # example: driver_bp.set_attribute("Attribute", str(value))
        # available attributes of the driver unit:
        # reaction_time_randomize (bool): if false: attribute "reaction_time_fix" (float) should be set,
        #                            otherwise the reaction time and the braking ratio of the driver will be 0.0;
        #                            if true: reaction time will be randomized in the range between reaction_time
        #                            _max and reaction_time_min
        # reaction_time_max (float)
        # reaction_time_min (float)
        # sensor_tick(float)
        driver_bp.set_attribute("sensor_tick", str(10.0))
        driver_bp.set_attribute("reaction_time_randomize", str(False))
        driver_bp.set_attribute("reaction_time_Fix", str(5.0))
        driver_bp.set_attribute("reaction_time_max", str(2.0))
        driver_bp.set_attribute("reaction_time_min", str(1.0))

        # Step 2: set the transformation of the driver unit like other sensors
        driver_location = carla.Location(0,0,0)
        driver_rotation = carla.Rotation(0,0,0)
        driver_transform = carla.Transform(driver_location,driver_rotation)

        # Step 3: spawn the driver unit to the world attached to the vehicle like other sensors
        driver = world.spawn_actor(driver_bp, driver_transform, attach_to=vehicle,attachment_type=carla.AttachmentType.Rigid)
        
        print('created %s' % driver.type_id)
        
        # Step 4: define the sensor callback: frame, timestamp, reaction_time, braking_ratio can be returned 
        def callback(driverunit):
            # output: reaction time
            print("Reactiontime:\n"+str(driverunit.reaction_time)+'\n')

            # output: braking ratio
            print("braking:\n"+str(driverunit.braking_ratio)+'\n')

        # Step 5: start sensor listening
        driver.listen(lambda driverunit: callback(driverunit))
        
        while True:
            world_snapshot = world.wait_for_tick()
            
            

    finally:
        pass



if __name__ == '__main__':
    
    try:
        main()
    except KeyboardInterrupt:
        pass
    finally:
        print('\ndone.')