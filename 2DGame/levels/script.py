# open level.level which is a yaml file  and print each component


import yaml
def main():
    with open('level01.level', "r+") as f:
        level = yaml.load(f, Loader=yaml.FullLoader)
        entities = level['Entities']
        
        for entity in entities:
            # Collidable to the bounding box component
            bounding_box = entity['Bounding Box Component']
            #add Collidable = false at the end of each entity
            name = entity['Tag'] 
            print(name)
            #if name is in [1-25] then add collidable = true
            try :
                if int(name) in range(1,26):
                    entity['Collidable'] = True
                else:
                    entity['Collidable'] = False
            except ValueError:
                entity['Collidable'] = False
            # pos = entity['Transform Component']['Position']
            # print (pos)
        # save the file and overwrite the old one
        f.seek(0)
        yaml.dump(level, f, default_flow_style=True)
if __name__ == '__main__':
    main()