package code.service;

import code.dao.UserRepository;
import code.entity.User;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.PageRequest;
import org.springframework.data.domain.Pageable;
import org.springframework.data.domain.Sort;
import org.springframework.stereotype.Service;
import org.springframework.web.multipart.MultipartFile;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

@Service
public class UserServiceImpl implements UserService {

    @Autowired
    private UserRepository dao;

    @Override
    public List<User> readAll() {
        return (List<User>) dao.findAll();
    }

    @Override
    public User readById(long id) {

        return dao.findById(id).orElse(new User());
    }

    @Override
    public void delete(long id) {
        dao.deleteById(id);
    }

    @Override
    public List<User> parseJson(MultipartFile file) {
        ObjectMapper mapper = new ObjectMapper();
        List<User> users = new ArrayList<>();
        try {

            users = mapper.readValue(new String(file.getBytes()), new TypeReference<List<User>>() {
            });
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
        System.out.println(users);
        users.forEach(o1->{
            User user = new User();
            user.setName(o1.getName());
            user.setScore(o1.getScore());
            dao.save(user);
        });
        return users;
    }

    @Override
    public List<User> getPage(long limit) {
        Pageable sortByScore = PageRequest.of(0, (int)limit, Sort.by("score").and(Sort.by("name")));
        Page<User> users = dao.findAll(sortByScore);
        System.out.println(users.getContent());
        return users.getContent();
    }


}
