using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace RpgAPI.Migrations
{
    public partial class InitialMigration : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.CreateTable(
                name: "Enemies",
                columns: table => new
                {
                    EnemyId = table.Column<int>(type: "int", nullable: false),
                    BattleTimeout = table.Column<int>(type: "int", nullable: false),
                    WanderAreaRadius = table.Column<float>(type: "real", nullable: false),
                    BattleAreaRadius = table.Column<float>(type: "real", nullable: false),
                    EnemyType = table.Column<int>(type: "int", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Enemies", x => x.EnemyId);
                });

            migrationBuilder.CreateTable(
                name: "GameMaps",
                columns: table => new
                {
                    GameMapId = table.Column<int>(type: "int", nullable: false),
                    WorldMapRow = table.Column<int>(type: "int", nullable: false),
                    WorldMapCol = table.Column<int>(type: "int", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_GameMaps", x => x.GameMapId);
                });

            migrationBuilder.CreateTable(
                name: "Players",
                columns: table => new
                {
                    PlayerId = table.Column<int>(type: "int", nullable: false),
                    StrengthPoints = table.Column<int>(type: "int", nullable: false),
                    IntelligencePoints = table.Column<int>(type: "int", nullable: false),
                    CriticalPoints = table.Column<int>(type: "int", nullable: false),
                    ExpPoints = table.Column<int>(type: "int", nullable: false),
                    PlayerType = table.Column<int>(type: "int", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Players", x => x.PlayerId);
                });

            migrationBuilder.CreateTable(
                name: "GameEntities",
                columns: table => new
                {
                    EntityId = table.Column<long>(type: "bigint", nullable: false),
                    Name = table.Column<string>(type: "nvarchar(10)", maxLength: 10, nullable: false),
                    Level = table.Column<int>(type: "int", nullable: false),
                    Gender = table.Column<int>(type: "int", nullable: false),
                    CurrentHealthPoints = table.Column<int>(type: "int", nullable: false),
                    MaxHealthPoints = table.Column<int>(type: "int", nullable: false),
                    AttackPoints = table.Column<int>(type: "int", nullable: false),
                    DefencePoints = table.Column<int>(type: "int", nullable: false),
                    CurrentManaPoints = table.Column<int>(type: "int", nullable: false),
                    MaxManaPoints = table.Column<int>(type: "int", nullable: false),
                    Speed = table.Column<float>(type: "real", nullable: false),
                    PositionX = table.Column<float>(type: "real", nullable: false),
                    PositionY = table.Column<float>(type: "real", nullable: false),
                    IsPlayer = table.Column<bool>(type: "bit", nullable: false),
                    GameMapId = table.Column<int>(type: "int", nullable: false),
                    PlayerId = table.Column<int>(type: "int", nullable: false),
                    NpcEnemyId = table.Column<int>(type: "int", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_GameEntities", x => x.EntityId);
                    table.ForeignKey(
                        name: "FK_GameEntities_Enemies_NpcEnemyId",
                        column: x => x.NpcEnemyId,
                        principalTable: "Enemies",
                        principalColumn: "EnemyId",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_GameEntities_GameMaps_GameMapId",
                        column: x => x.GameMapId,
                        principalTable: "GameMaps",
                        principalColumn: "GameMapId",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_GameEntities_Players_PlayerId",
                        column: x => x.PlayerId,
                        principalTable: "Players",
                        principalColumn: "PlayerId",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateIndex(
                name: "IX_GameEntities_GameMapId",
                table: "GameEntities",
                column: "GameMapId");

            migrationBuilder.CreateIndex(
                name: "IX_GameEntities_NpcEnemyId",
                table: "GameEntities",
                column: "NpcEnemyId");

            migrationBuilder.CreateIndex(
                name: "IX_GameEntities_PlayerId",
                table: "GameEntities",
                column: "PlayerId");
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "GameEntities");

            migrationBuilder.DropTable(
                name: "Enemies");

            migrationBuilder.DropTable(
                name: "GameMaps");

            migrationBuilder.DropTable(
                name: "Players");
        }
    }
}
