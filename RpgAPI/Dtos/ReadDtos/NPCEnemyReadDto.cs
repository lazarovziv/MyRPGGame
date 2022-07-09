using RpgAPI.Enums;

namespace RpgAPI.Dtos.ReadDtos
{
    public class NPCEnemyReadDto
    {
        public int EnemyId { get; set; }
        public int BattleTimeout { get; set; }
        public float WanderAreaRadius { get; set; }
        public float BattleAreaRadius { get; set; }
        public EnemyType EnemyType { get; set; }
    }
}