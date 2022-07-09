namespace RpgAPI.Dtos.ReadDtos
{
    public class GameEntityReadDto
    {
        public long EntityId { get; set; }
        public string? Name { get; set; }
        public int Level { get; set; }
        public int Gender { get; set; }
        public int CurrentHealthPoints { get; set; }
        public int MaxHealthPoints { get; set; }
        public int AttackPoints { get; set; }
        public int DefencePoints { get; set; }
        public int CurrentManaPoints { get; set; }
        public int MaxManaPoints { get; set; }
        public float Speed { get; set; }
        public float PositionX { get; set; }
        public float PositionY { get; set; }
        public bool IsPlayer { get; set; }
        public int GameMapId { get; set; }
        public GameMapReadDto? GameMap { get; set; }
        public int? PlayerId { get; set; }
        public PlayerReadDto? Player { get; set; }
        public int? NpcEnemyId { get; set; }
        public NPCEnemyReadDto? NpcEnemy { get; set; }
    }
}
