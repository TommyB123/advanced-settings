CREATE TABLE `changeme` (
  `id` int(11) NOT NULL,
  `entityid` int(11) NOT NULL,
  `key` varchar(32) NOT NULL,
  `value` int(11) NOT NULL,
  `extra` longtext CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL DEFAULT '{}' CHECK (json_valid(`extra`))
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;

ALTER TABLE `changeme`
  ADD PRIMARY KEY (`id`),
  ADD KEY `setting` (`key`),
  ADD KEY `entityid` (`entityid`);

ALTER TABLE `changeme`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;