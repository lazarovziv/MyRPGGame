using RpgAPI.Enums;

namespace RpgAPI.Dtos.ReadDtos
{
    public class PlayerReadDto
    {
        public int PlayerId { get; set; }
        public int StrengthPoints { get; set; }
        public int IntelligencePoints { get; set; }
        public int CriticalPoints { get; set; }
        public int ExpPoints { get; set; }
        public PlayerType PlayerType { get; set; }
    }
}
