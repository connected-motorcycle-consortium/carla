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

        # create driver blueprint from carla library
        driver_bp = world.get_blueprint_library().find('sensor.other.driver')

        # Step 1: you can set attributes of the driver unit here
        # example: driver_bp.set_attribute("Attribute", str(value))
        # available attributes of the driver unit:
        # reaction_time_random (bool): if false: attribute "reaction_time_fix" (float) can be set
        # reaction_time_max (float)
        # reaction_time_min (float)
        # braking_ratio_random (bool): if false: attribute "braking_ratio_fix" (float, range: 0.0-1.0) can be set
        # sensor_tick(float)
        driver_bp.set_attribute("sensor_tick", str(1.0))
        driver_bp.set_attribute("reaction_time_random", str(True))
        driver_bp.set_attribute("reaction_time_fix", str(5.0))
        driver_bp.set_attribute("reaction_time_max", str(2.0))
        driver_bp.set_attribute("reaction_time_min", str(1.0))
        driver_bp.set_attribute("braking_ratio_random", str(True))
        driver_bp.set_attribute("braking_ratio_fix", str(1.0))

        driver_location = carla.Location(0, 0, 0)
        driver_rotation = carla.Rotation(0, 0, 0)
        driver_transform = carla.Transform(driver_location, driver_rotation)

        driver = world.spawn_actor(driver_bp, driver_transform, attach_to=vehicle, attachment_type=carla.AttachmentType.Rigid)
        
        print('created %s' % driver.type_id)
        
        def callback(DriverUnit):
            # output: reaction time
            print("Reaction time:\n"+str(DriverUnit.reaction_time)+'\n')

            # output: braking ratio
            print("braking ratio:\n"+str(DriverUnit.braking_ratio)+'\n')

        driver.listen(lambda DriverUnit: callback(DriverUnit))
        
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