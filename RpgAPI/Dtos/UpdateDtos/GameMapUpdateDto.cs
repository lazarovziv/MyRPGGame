using System.ComponentModel.DataAnnotations;

namespace RpgAPI.Dtos.UpdateDtos
{
    public class GameMapUpdateDto
    {
        [Required]
        public int GameMapId { get; set; }
        [Required]
        public int WorldMapRow { get; set; }
        [Required]
        public int WorldMapCol { get; set; }
    }
}