import yaml

def run_player(loop_count):
    for _ in range(loop_count):
        print("Running player logic...")
        import vision_debug
        vision_debug.run()
        # 在这里放置实际的 player 逻辑
        # 例如：import player; player.run()


def run_main(loop_count):
    for _ in range(loop_count):
        print("Running main logic...")
        import main
        main.begin()
        # 在这里放置实际的 main 逻辑
        # 例如：import main; main.run()


def main():
    # 读取配置文件
    with open('sorting_robot.yaml', 'r') as file:
        config = yaml.safe_load(file)

    for item in config:
        if 'player' in item:
            run_player(item['player']['loop'])
        elif 'main' in item:
            run_main(item['main']['loop'])


if __name__ == "__main__":
    main()
